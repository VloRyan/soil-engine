
#include "stage/scene/scene.h"

#include <ranges>
#include <stdexcept>

#include "stage/scene/component/instance_data.h"
#include "stage/scene/render/plain.h"
#include "stage/stage.h"
#include "video/render/step/bind_buffer.h"
#include "video/render/step/render.h"

namespace soil::stage::scene {
    constexpr auto SCENE_NODE_OFFSET = 100000;

    Scene::Scene(Stage *stage) :
        Node(stage->AddScene(this), Type::Scene, {}, ".::SCENE::."), stage_(stage), viewer_(nullptr),
        dirtyNodesPtr_(&dirtyNodes_), renderContainer_(new video::render::Container()),
        nextId_(GetId() * SCENE_NODE_OFFSET), worldTransform_(1), uboMatricesBindingTarget_(-1), pipeline_(nullptr) {
        SetScene(this);
        renderFeatures_[render::Type::Plain] = new render::Plain();
        renderFeatures_[render::Type::Instancing] = new render::Instancing();

        pipeline_ = new video::render::Pipeline("renderScene" + std::to_string(GetId()), renderContainer_);
        pipeline_->InsertStep(
            new video::render::step::Render("renderOpaque", {.Blending = false}, {.Blend = false, .DepthTest = true}));
        pipeline_->InsertStep(
            new video::render::step::Render("renderNonOpaque", {.Blending = true}, {.Blend = true, .DepthTest = true}));
    }

    void Scene::RemoveChild(Node *node) {
        NodeRemoved(node);
        Node::RemoveChild(node);
    }

    void Scene::Render(video::render::State &state) {
        if (pipeline_ == nullptr || viewer_ == nullptr) {
            return;
        }
        for (auto *feature : renderFeatures_ | std::views::values) {
            feature->Update(renderContainer_);
        }
        if (uboMatricesBindingTarget_ != -1) {
            state.WriteUbo(uboMatricesBindingTarget_, [this](video::buffer::Cursor *cursor) {
                const auto proj = viewer_->GetProjectionMatrix();
                const auto view = viewer_->GetViewMatrix();
                const auto pvMatrix = proj * view;
                cursor->Write(proj);
                cursor->Write(view);
                cursor->Write(glm::inverse(view));
                cursor->Write(pvMatrix);
            });
        }
        if (beforeRenderViewer_ != nullptr) {
            beforeRenderViewer_(viewer_, pipeline_);
        }
        pipeline_->Run(state);
    }

    void Scene::Update() {
        for (auto *node : activeUpdateNodes_) {
            node->Update();
        }
        auto *lastDirtyNodes = dirtyNodesPtr_;
        if (dirtyNodesPtr_ == &dirtyNodes_) {
            dirtyNodesPtr_ = &dirtyNodes2_;
        } else {
            dirtyNodesPtr_ = &dirtyNodes_;
        }
        for (auto *node : *lastDirtyNodes) {
            if (!node->IsDirty() || node->GetType() == Type::Viewer || node->GetUpdateType() == UpdateType::Active) {
                continue;
            }
            if (auto *topDirty = computeTopDirtyNode(node); topDirty != nullptr) {
                topDirty->Update();
            }
        }
        lastDirtyNodes->clear();
        if (viewer_ != nullptr) {
            viewer_->Update();
        }
    }

    void Scene::Handle(const event::Node &event) {
        switch (event.GetChangeType()) {
        case event::Node::ChangeType::State:
            NodeStateChanged(event.GetOrigin());
            break;
        case event::Node::ChangeType::Created:
            NodeAdded(event.GetOrigin());
            break;
        case event::Node::ChangeType::ChildAdded:
            NodeAdded(event.GetChangedNode());
            break;
        case event::Node::ChangeType::Deleted:
            NodeRemoved(event.GetOrigin());
            break;
        case event::Node::ChangeType::ChildRemoved:
            NodeRemoved(event.GetChangedNode());
            break;
        case event::Node::ChangeType::UpdateType:
            switch (event.GetOrigin()->GetUpdateType()) {
            case UpdateType::Active:
                activeUpdateNodes_.emplace_back(event.GetOrigin());
                break;
            case UpdateType::Passive:
                for (auto itr = activeUpdateNodes_.begin(); itr != activeUpdateNodes_.end(); ++itr) {
                    if (*itr == event.GetOrigin()) {
                        activeUpdateNodes_.erase(itr);
                        break;
                    }
                }
                break;
            }
        default:; // do nothing
        }
    }

    void Scene::NodeStateChanged(Node *node) {
        if (node->IsDirty()) {
            dirtyNodesPtr_->push_back(node);
        } else if (node->GetState() == State::Delete) {
            delete node;
        }
    }

    void Scene::NodeAdded(Node *node) {
        switch (node->GetType()) {
        case Type::Viewer:
            {
                auto *viewer = dynamic_cast<viewer::Node *>(node);
                if (viewer == nullptr) {
                    return;
                }
                if (viewer_ != nullptr) {
                    throw std::runtime_error("Viewer already exists");
                }
                viewer_ = viewer;
                break;
            }
        default:
            if (node->GetUpdateType() == UpdateType::Active) {
                activeUpdateNodes_.push_back(node);
            }
        }
        dirtyNodesPtr_->push_back(node);
        if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Window)]) {
            windowEventReceiverNodes_.push_back(node);
        }
        if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Input)]) {
            inputEventReceiverNodes_.push_back(node);
        }
    }

    void Scene::NodeRemoved(const Node *node) {
        switch (node->GetType()) {
        case Type::Viewer:
            viewer_ = nullptr;
            break;
        default:
            if (node->GetUpdateType() == UpdateType::Active) {
                for (auto itr = activeUpdateNodes_.begin(); itr != activeUpdateNodes_.end(); ++itr) {
                    if (*itr == node) {
                        activeUpdateNodes_.erase(itr);
                        break;
                    }
                }
            }
        }
        if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Window)]) {
            for (auto itr = windowEventReceiverNodes_.begin(); itr != windowEventReceiverNodes_.end(); ++itr) {
                if (*itr == node) {
                    windowEventReceiverNodes_.erase(itr);
                    break;
                }
            }
        }
        if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Input)]) {
            for (auto itr = inputEventReceiverNodes_.begin(); itr != inputEventReceiverNodes_.end(); ++itr) {
                if (*itr == node) {
                    inputEventReceiverNodes_.erase(itr);
                    break;
                }
            }
        }
    }

    void Scene::SetBeforeRenderViewer(
        const std::function<void(viewer::Node *node, video::render::Pipeline *pipeline)> &before_render_viewer) {
        beforeRenderViewer_ = before_render_viewer;
    }

    render::Feature *Scene::GetRenderFeature(const render::Type type) {
        return renderFeatures_[type];
    }

    render::Instancing *Scene::Instancing() {
        return dynamic_cast<render::Instancing *>(GetRenderFeature(render::Type::Instancing));
    }

    viewer::Node *Scene::GetViewer() const {
        return viewer_;
    }

    void Scene::Handle(const event::Component &event) {
        switch (event.GetChangeType()) {
        case event::Component::ChangeType::Added:
            ComponentAdded(event.GetOrigin());
            break;
        case event::Component::ChangeType::Removed:
            ComponentRemoved(event.GetOrigin());
            break;
        case event::Component::ChangeType::State:
            ComponentStateChanged(event.GetOrigin());
            break;
        }
    }

    void Scene::ComponentAdded(component::Component *component) {
        switch (component->GetType()) {
        case component::Component::Type::Visual:
            if (auto *vComp = dynamic_cast<component::VisualComponent *>(component); vComp != nullptr) {
                renderFeatures_[vComp->GetRenderType()]->ComponentAdded(vComp);
            }
            break;
        default:; // do nothing
        }
    }

    void Scene::ComponentRemoved(component::Component *component) {
        switch (component->GetType()) {
        case component::Component::Type::Visual:
            if (auto *vComp = dynamic_cast<component::VisualComponent *>(component); vComp != nullptr) {
                renderFeatures_[vComp->GetRenderType()]->ComponentRemoved(vComp);
            }
            break;
        default:;
        }
    }

    void Scene::ComponentStateChanged(component::Component *component) {
        if (component->GetState() != component::Component::State::Normal) {
            return;
        }
        switch (component->GetType()) {
        case component::Component::Type::Visual:
            if (auto *vComp = dynamic_cast<component::VisualComponent *>(component); vComp != nullptr) {
                renderFeatures_[vComp->GetRenderType()]->ComponentChanged(vComp);
            }
            break;
        default:; // do nothing
        }
    }

    Node *Scene::computeTopDirtyNode(Node *current) {
        if (current->GetParent() != nullptr && current->GetParent()->IsDirty()) {
            return computeTopDirtyNode(current->GetParent());
        }
        return current;
    }

    int Scene::GetUboMatricesBindingTarget() const {
        return uboMatricesBindingTarget_;
    }

    void Scene::SetUboMatricesBindingTarget(const int ubo_matrices_binding_target) {
        uboMatricesBindingTarget_ = ubo_matrices_binding_target;
    }

    video::render::Pipeline *Scene::GetPipeline() const {
        return pipeline_;
    }

    void Scene::SetPipeline(video::render::Pipeline *const pipeline) {
        pipeline_ = pipeline;
    }

    void Scene::forEachChild(const Node *node, const std::function<void(Node *child)> &func) {
        for (auto *const child : node->GetChildren()) {
            func(child);
            forEachChild(child, func);
        }
    }

    void Scene::Handle(const input::Event &event) {
        for (auto *const node : inputEventReceiverNodes_) {
            node->Handle(event);
        }
    }

    void Scene::Handle(const WindowEvent &event) {
        for (auto *const node : windowEventReceiverNodes_) {
            node->Handle(event);
        }
    }

    int Scene::GetNextId() {
        const auto nextId = ++nextId_;
        if (nextId % SCENE_NODE_OFFSET == 0) {
            throw std::runtime_error("nextId reached next scene range");
        }
        return nextId;
    }

    void Scene::AddChild(Node *child) {
        Node::AddChild(child);
    }

    Resources &Scene::GetResources() const {
        return stage_->GetResources();
    }

    Stage *Scene::GetStage() const {
        return stage_;
    }
} // namespace soil::stage::scene
