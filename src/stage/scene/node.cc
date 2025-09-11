
#include "stage/scene/node.h"

#include <cstring>
#include <debug/deque>
#include <plog/Log.h>
#include <ranges>
#include <stdexcept>
#include <utility>

#include "stage/event/node.h"
#include "stage/scene/scene.h"

namespace soil::stage::scene {
    Node::Node(const Type type) :
        parent_(nullptr), type_(type), state_(State::Normal), updateType_(UpdateType::Passive) {}

    Node::~Node() {
        for (auto* child : children_) {
            child->SetParent(nullptr);
            delete child;
        }
        for (const auto& comps : components_ | std::views::values) {
            for (auto* comp : comps) {
                comp->SetParent(nullptr);
                const auto removedEvent = event::Component(comp, event::Component::ChangeType::Removed);
                Observable::fire(event::Node::MakeComponentEvent(this, removedEvent));
                delete comp;
            }
        }
        if (auto* parent = GetParent(); parent != nullptr) {
            parent->RemoveChild(this);
        }
    }

    void Node::SetParent(Node* parent) {
        parent_ = parent;
    }

    void Node::addChild(Node* node) {
        if (node->GetParent() == this) {
            return;
        }
        node->SetParent(this);
        node->SetDirty(DirtyImpact::Transform);
        children_.push_back(node);
        // TODO: Only children
        SetDirty(DirtyImpact::Dependents);
        fire(event::Node::MakeChildAddedEvent(this, node));
    }

    void Node::RemoveChild(Node* node) {
        for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
            if (*itr == node) {
                if (!isScene()) {
                    fire(event::Node::MakeChildRemovedEvent(this, node));
                }
                node->SetParent(nullptr);
                children_.erase(itr);
                break;
            }
        }
        SetDirty(DirtyImpact::Self);
    }

    bool Node::isScene() const {
        return GetType() == Type::Scene;
    }

    void Node::SetUpdateType(const UpdateType type) {
        if (type == updateType_) {
            return;
        }
        updateType_ = type;
        fire(event::Node(this, event::Node::ChangeType::UpdateType));
    }

    Node::UpdateType Node::GetUpdateType() const {
        return updateType_;
    }

    Node::Type Node::GetType() const {
        return type_;
    }

    void Node::SetPosition(const glm::vec3& pos) {
        if (pos == GetPosition()) {
            return;
        }
        Object3d::SetPosition(pos);
        SetDirty(DirtyImpact::Transform);
    }

    void Node::SetDirection(const glm::vec3& direction) {
        if (direction == GetDirection()) {
            return;
        }
        Object3d::SetDirection(direction);
        SetDirty(DirtyImpact::Transform);
    }

    void Node::SetRight(const glm::vec3& right) {
        if (right == GetRight()) {
            return;
        }
        Object3d::SetRight(right);
        SetDirty(DirtyImpact::Transform);
    }

    void Node::SetUp(const glm::vec3& up) {
        if (up == GetUp()) {
            return;
        }
        Object3d::SetUp(up);
        SetDirty(DirtyImpact::Transform);
    }

    void Node::SetLocalTransform(const glm::mat4& transform) {
        if (transform == GetLocalTransform()) {
            return;
        }
        Object3d::SetLocalTransform(transform);
        SetDirty(DirtyImpact::Transform);
    }

    void Node::Update() {
        if (!IsDirty()) {
            return;
        }
        UpdateDirty();
    }

    void Node::UpdateDirty() {
        if (IsDirtyImpact(DirtyImpact::Transform)) {
            if (!isScene() && GetParent() != nullptr) {
                ComputeWorldTransform(GetParent()->GetWorldTransform());
            }
            ForEachComponent([this](component::Component* component) {
                component->SetDirty();
                component->UpdateTransform(GetWorldTransform());
                component->Update();
            });
            for (auto* child : children_) {
                child->SetDirty(DirtyImpact::Transform);
                child->UpdateDirty();
            }
        } else {
            if (IsDirtyImpact(DirtyImpact::Components) || IsDirtyImpact(DirtyImpact::Dependents)) {
                ForEachComponent([](component::Component* component) { component->Update(); });
            }
            if (IsDirtyImpact(DirtyImpact::Dependents)) {
                for (auto* child : children_) {
                    child->SetDirty(DirtyImpact::Dependents);
                    child->UpdateDirty();
                }
            }
        }

        for (auto* comp : addedComponents_) {
            if (comp->GetParent() != nullptr) {
                comp->GetParent()->RemoveComponent(comp);
            }
            comp->SetParent(this);
            comp->AddListener(this);
            comp->UpdateTransform(GetWorldTransform());
            comp->Update();
            const auto compTypeIndex = static_cast<std::int8_t>(comp->GetType());

            components_[compTypeIndex].push_back(comp);
            const auto addedEvent = event::Component(comp, event::Component::ChangeType::Added);
            Observable::fire(event::Node::MakeComponentEvent(this, addedEvent));
        }
        addedComponents_.clear();
        SetState(State::Normal);
        dirtyImpacts_ = 0;
    }

    bool Node::IsDirty() const {
        return state_ == State::Dirty;
    }

    Node::State Node::GetState() const {
        return state_;
    }

    bool Node::IsState(const State state) const {
        return state_ == state;
    }

    bool Node::IsDirtyImpact(const DirtyImpact cause) const {
        return IsDirtyImpact(dirtyImpacts_, cause);
    }

    void Node::SetState(const State state) {
        if (state_ == state) {
            return;
        }
        state_ = state;
        fire(event::Node(this, event::Node::ChangeType::State));
    }

    void Node::SetDirty(DirtyImpact cause) {
        dirtyImpacts_[static_cast<std::int8_t>(cause)] = true;
        SetState(State::Dirty);
    }

    Node* Node::GetParent() const {
        return parent_;
    }

    const std::vector<Node*>& Node::GetChildren() const {
        return children_;
    }

    void Node::addComponent(component::Component* comp) {
#ifdef DEBUG
        if (comp == nullptr) {
            throw std::invalid_argument("component cannot be null");
        }
#endif
        if (this == comp->GetParent()) {
            return;
        }
        addedComponents_.push_back(comp);
        SetDirty(DirtyImpact::Components);
    }

    void Node::RemoveComponent(component::Component* comp) {
        if (this != comp->GetParent()) {
            for (auto itr = addedComponents_.begin(); itr != addedComponents_.end(); ++itr) {
                if (comp == *itr) {
                    addedComponents_.erase(itr);
                    return;
                }
            }
            return;
        }
        const auto typeIndex = static_cast<std::int8_t>(comp->GetType());

        for (auto itr = components_[typeIndex].begin(); itr != components_[typeIndex].end(); ++itr) {
            if (comp == *itr) {
                components_[typeIndex].erase(itr);
                comp->SetParent(nullptr);
                comp->RemoveListener(this);
                break;
            }
        }
        const auto removedEvent = event::Component(comp, event::Component::ChangeType::Removed);
        Observable::fire(event::Node::MakeComponentEvent(this, removedEvent));
    }

    bool Node::HasComponent(component::Component::Type type) const {
        return components_.contains(static_cast<std::int8_t>(type));
    }

    void Node::Handle(const event::Component& event) {
        if (event.GetOrigin()->GetParent() != this) {
            return;
        }
        if (event.GetChangeType() == event::Component::ChangeType::State) {
            for (const auto* justAddedComp : addedComponents_) {
                if (event.GetOrigin() == justAddedComp) {
                    return;
                }
            }
            if (event.GetOrigin()->IsDirty()) {
                SetDirty(DirtyImpact::Components);
            }
        }
        Observable::fire(event::Node::MakeComponentEvent(this, event));
    }

    bool Node::GetReceiverType(ReceiverType type) const {
        if (type == ReceiverType::None) {
            return receiveTypeFlags_ == 0;
        }
        return receiveTypeFlags_[static_cast<std::int8_t>(type) - 1];
    }

    void Node::SetReceiverType(ReceiverType type, const bool value) {
        if (type == ReceiverType::None) {
            receiveTypeFlags_ = 0;
            return;
        }
        receiveTypeFlags_[static_cast<std::int8_t>(type) - 1] = value;
    }

    std::bitset<4> Node::GetDirtyImpacts() const {
        return dirtyImpacts_;
    }

    void Node::ForEachComponent(const std::function<void(component::Component*)>& func,
                                component::Component::Type ofType) const {
        if (ofType == component::Component::Type::Any) {
            for (const auto& comps : components_ | std::views::values) {
                for (auto* comp : comps) {
                    func(comp);
                }
            }
        } else {
            const auto typeIndex = static_cast<std::int8_t>(ofType);
            const auto pos = components_.find(typeIndex);
            if (pos == components_.end() || pos->second.empty()) {
                return;
            }
            for (auto* comp : pos->second) {
                func(comp);
            }
        }
    }

    void Node::GetComponents(std::vector<component::Component*>& comps, const component::Component::Type type) const {
        if (type == component::Component::Type::Any) {
            for (const auto& c : components_ | std::views::values) {
                comps.insert(comps.end(), c.begin(), c.end());
            }
            return;
        }
        const auto typeIndex = static_cast<std::int8_t>(type);
        const auto pos = components_.find(typeIndex);
        if (pos == components_.end() || pos->second.empty()) {
            return;
        }
        comps.insert(comps.end(), pos->second.begin(), pos->second.end());
    }

    component::Component* Node::GetFirstComponent(component::Component::Type type) const {
        auto itr = components_.end();
        if (type == component::Component::Type::Any) {
            itr = components_.begin();
        } else {
            const auto typeIndex = static_cast<std::int8_t>(type);
            itr = components_.find(typeIndex);
        }
        if (itr == components_.end() || itr->second.empty()) {
            return nullptr;
        }
        return itr->second.front();
    }

    bool Node::IsDirtyImpact(const std::bitset<4>& dirtyImpacts, DirtyImpact impact) {
        return dirtyImpacts[static_cast<std::int8_t>(impact)];
    }

    void Node::ForEachChild(const Node* node, const std::function<void(Node* child)>& func) {
        if (node->GetChildren().empty()) {
            return;
        }
        constexpr auto defaultBufferSize = 8;
        auto bufferSize = std::max(defaultBufferSize, static_cast<int>(node->GetChildren().size()));
        auto buffer = new Node*[bufferSize];
        std::memcpy(buffer, &node->GetChildren().front(), node->GetChildren().size() * sizeof(Node*));
        auto cursor = 0;
        auto dataEnd = node->GetChildren().size();
        auto freeCapacity = bufferSize - dataEnd;
        while (cursor < dataEnd) {
            func(buffer[cursor]);
            auto& c = buffer[cursor]->GetChildren();
            const auto elemsToPush = c.size();
            if (elemsToPush == 0) {
                cursor++;
                continue;
            }
            if (freeCapacity < elemsToPush) {
                if (cursor + 1 < dataEnd) {
                    std::memcpy(buffer, &buffer[cursor + 1], (dataEnd - 1 - cursor) * sizeof(Node*));
                    dataEnd -= cursor + 1;
                    cursor = -1;
                    freeCapacity = bufferSize - dataEnd;
                } else {
                    dataEnd = 0;
                    cursor = -1;
                    freeCapacity = bufferSize;
                }
                if (freeCapacity < elemsToPush) {
                    bufferSize += static_cast<int>(elemsToPush);
                    const auto newBuffer = new Node*[bufferSize];
                    memcpy(newBuffer, buffer, dataEnd * sizeof(Node*));
                    delete[] buffer;
                    buffer = newBuffer;
                    freeCapacity = bufferSize - dataEnd;
                }
            }
            std::memcpy(buffer + dataEnd, &c.front(), elemsToPush * sizeof(Node*));
            dataEnd += elemsToPush;
            freeCapacity -= elemsToPush;
            cursor++;
        }
        delete[] buffer;
    }

} // namespace soil::stage::scene
