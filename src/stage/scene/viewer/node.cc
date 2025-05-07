#include <algorithm>

#include "stage/scene/viewer/node.h"

#include <stdexcept>

#include "stage/scene/space/quad_tree.h"
#include "video/manager.h"

namespace soil::stage::scene::viewer {
    Node::Node(scene::Node *parent) :
        scene::Node(nullptr, Type::Viewer), pipeline_(nullptr), viewChanged_(true), quadTree_(nullptr),
        stateDef_({.Blend = false, .DepthTest = true, .StencilTest = false}), uboMatricesBindingTarget_(-1) {
        SetParent(parent);
    }

    Node::Node(scene::Node *parent, const std::vector<scene::Node::ReceiverType> &receiverTypes) :
        scene::Node(nullptr, Type::Viewer, receiverTypes, ""), pipeline_(nullptr), viewChanged_(true),
        quadTree_(nullptr), stateDef_({.Blend = false, .DepthTest = true, .StencilTest = false}),
        uboMatricesBindingTarget_(-1) {
        SetParent(parent);
    }


    video::render::Pipeline *Node::GetPipeline() const { return pipeline_; }

    bool Node::IsViewChanged() const { return viewChanged_; }

    void Node::AddRenderable(component::RenderableInstance *renderable) {
        const auto *mesh = renderable->GetMesh();
        auto *shader = renderable->GetShader();
        auto batchKey = renderable->GetBatchKey();
        if (batchKey.empty()) {
            batchKey = video::render::Batch::MakeKey(*mesh, shader, renderable->IsOpaque());
            renderable->SetBatchKey(batchKey);
        }
        const auto itr = batchMap_.find(batchKey);
        video::render::Batch *batch = nullptr;
        if (itr != batchMap_.end()) {
            batch = itr->second;
        } else {
            batch = new video::render::Batch(*mesh, shader, renderable->IsOpaque());
            batchMap_.insert(std::make_pair(batchKey, batch));
            renderables_.emplace_back(batch);
        }
        batch->AddNewInstance(renderable);
    }

    void Node::AddChangedRenderable(component::RenderableInstance *renderable) {
        auto batchKey = renderable->GetBatchKey();
        if (batchKey.empty()) {
            const auto *mesh = renderable->GetMesh();
            const auto *shader = renderable->GetShader();
            batchKey = video::render::Batch::MakeKey(*mesh, shader, renderable->IsOpaque());
            renderable->SetBatchKey(batchKey);
            auto found = false;
            for (const auto &pair : batchMap_) {
                if (pair.second->RemoveInstance(renderable)) {
                    found = true;
                    break;
                }
            }
            // TODO
            /*if (!found) {
                throw std::runtime_error("Failed to remove instance from batch.");
            }*/
            AddRenderable(renderable);
            return;
        }
        const auto itr = batchMap_.find(batchKey);
        if (itr == batchMap_.end()) {
            return;
        }
        itr->second->AddChangedInstance(renderable);
    }

    void Node::RemoveRenderable(component::RenderableInstance *renderable) {
        const auto batchKey = renderable->GetBatchKey();
        if (batchKey.empty()) {
            for (const auto &pair : batchMap_) {
                if (pair.second->RemoveInstance(renderable)) {
                    break;
                }
            }
        }
        const auto itr = batchMap_.find(batchKey);
        if (itr == batchMap_.end()) {
            return;
        }
        itr->second->RemoveInstance(renderable);
    }

    void Node::SetQuadTree(space::QuadTree *quadTree) { quadTree_ = quadTree; }

    void Node::updateVisibilityOnTreeNode(const int index) {
        const auto *treeNode = quadTree_->GetNode(index);
        std::array points{
            glm::vec3(treeNode->Min.x, 0.0F, treeNode->Min.y), glm::vec3(treeNode->Max.x, 0.0F, treeNode->Max.y),
            glm::vec3(treeNode->Min.x, 0.0F, treeNode->Max.y), glm::vec3(treeNode->Max.x, 0.0F, treeNode->Min.y)};
        const auto isVisible = GetFrustum()->IsVisible(std::vector<glm::vec3>{std::begin(points), std::end(points)}, 4);
        if (!isVisible && nodesVisibility_[index] == isVisible) {
            return;
        }
        if (nodesVisibility_[index] != isVisible) {
            for (const auto *volume : treeNode->Contained) {
                const auto *node = volume->GetParent();
                for (auto renderables = node->GetComponents(component::Component::Type::Renderable);
                     auto *renderable : renderables) {
                    auto *instance = dynamic_cast<component::RenderableInstance *>(renderable);
                    const auto *mesh = instance->GetMesh();
                    const auto *shader = instance->GetShader();
                    const auto batchKey = video::render::Batch::MakeKey(*mesh, shader, instance->IsOpaque());
                    const auto itr = batchMap_.find(batchKey);
                    if (itr == batchMap_.end()) {
                        continue;
                    }
                    if (isVisible) {
                        itr->second->AddNewInstance(instance);
                    } else {
                        itr->second->RemoveInstance(instance);
                    }
                }
            }
            nodesVisibility_[index] = isVisible;
        }
        if (nodesVisibility_[index]) {
            if (const int firstChildIndex = treeNode->FirstChildIndex; firstChildIndex != -1) {
                for (int j = 0; j < 4; ++j) {
                    updateVisibilityOnTreeNode(firstChildIndex + j);
                }
            }
        }
    }

    void Node::Update() {
        scene::Node::Update();
        if (quadTree_ != nullptr && GetFrustum() != nullptr) {
            if (nodesVisibility_.empty()) {
                nodesVisibility_.resize(quadTree_->GetNodeCount());
                std::ranges::fill(nodesVisibility_, false);
            }
            updateVisibilityOnTreeNode(0);
        }
        for (const auto &itr : batchMap_) {
            itr.second->Update(GetPosition());
        }
    }

    void Node::Render(video::render::State &state) {
        state.Apply(stateDef_);
        if (uboMatricesBindingTarget_ != -1) {
            state.WriteUbo(uboMatricesBindingTarget_, [this](video::buffer::Cursor *cursor) {
                const auto proj = GetProjectionMatrix();
                const auto view = GetViewMatrix();
                const auto pvMatrix = proj * view;
                cursor->Write(proj);
                cursor->Write(view);
                cursor->Write(glm::inverse(view));
                cursor->Write(pvMatrix);
            });
        }
        GetPipeline()->Render(renderables_, state);
    }

    video::render::StateDef Node::GetStateDef() const { return stateDef_; }

    void Node::SetStateDef(const video::render::StateDef &stateDef) { stateDef_ = stateDef; }

    int Node::GetUboMatricesBindingTarget() const { return uboMatricesBindingTarget_; }

    void Node::SetUboMatricesBindingTarget(const int uboMatricesBindingTarget) {
        uboMatricesBindingTarget_ = uboMatricesBindingTarget;
    }
} // namespace soil::stage::scene::viewer
