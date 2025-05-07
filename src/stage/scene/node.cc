
#include "stage/scene/node.h"

#include <utility>

#include "stage/event/node.h"

#include "stage/scene/scene.h"

namespace soil::stage::scene {
    Node::Node(const int id, const Type type, const std::vector<ReceiverType> &receiveTypeFlags, std::string name) :
        id_(id), parent_(nullptr), scene_(nullptr), name_(std::move(name)), type_(type), state_(State::Normal),
        updateType_(UpdateType::Passive) {
        for (auto flag : receiveTypeFlags) {
            receiveTypeFlags_[static_cast<int>(flag)] = true;
        }
    }

    Node::Node(Node *parent, const Type type, const std::vector<ReceiverType> &receiveTypeFlags, std::string name) :
        Node(0, type, receiveTypeFlags, std::move(name)) {
        SetParent(parent);
    }

    Node::~Node() {
        if (id_ != 0 && parent_ != nullptr) {
            parent_->RemoveChild(this);
        }
        for (auto *child : children_) {
            child->parent_ = nullptr;
            delete child;
        }
        for (auto &compsOfType : components_) {
            for (auto *comp : compsOfType) {
                comp->SetParent(nullptr);
                const auto evnt = event::Component(comp, event::Component::ChangeType::Removed);
                if (scene_ != nullptr) {
                    scene_->Handle(evnt);
                }
                delete comp;
            }
        }
    }

    void Node::SetScene(Scene *root) { scene_ = root; }

    void Node::AddChild(Node *node) {
        if (node->parent_ == this) {
            return;
        }
        node->parent_ = this;
        children_.push_back(node);
        fire(event::Node(this, event::Node::ChangeType::ChildAdded).WithNode(node));
    }

    void Node::RemoveChild(Node *node) {
        for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
            if (*itr == node) {
                if (!isScene()) {
                    fire(event::Node(this, event::Node::ChangeType::ChildRemoved).WithNode(node));
                }
                node->parent_ = nullptr;
                children_.erase(itr);
                break;
            }
        }
    }

    bool Node::isScene() const { return type_ == Type::Scene; }

    void Node::SetUpdateType(const UpdateType type) {
        if (type == updateType_) {
            return;
        }
        updateType_ = type;
        SetDirty(DirtyCauses::UpdateType);
    }

    Node::UpdateType Node::GetUpdateType() const { return updateType_; }

    int Node::GetId() const { return id_; }

    void Node::SetParent(Node *parent) {
        if (parent_ == parent) {
            return;
        }
        if (parent != nullptr) {
            if (parent_ != nullptr) {
                parent_->RemoveChild(this);
            }
#ifdef DEBUG
            if (parent->GetScene() == nullptr) {
                throw std::invalid_argument("parent->root is null");
            }
#endif
            scene_ = parent->scene_;
            id_ = scene_->GetNextId();
            parent->AddChild(this);
            parent_ = parent;
        } else {
            parent_ = nullptr;
            scene_ = nullptr;
            id_ = 0;
        }
    }

    std::string Node::GetName() const { return name_; }

    void Node::SetName(const std::string &name) { name_ = name; }

    Scene *Node::GetScene() const { return scene_; }

    void Node::SetPosition(const glm::vec3 pos) {
        const auto relPos = pos;
        Object3d::SetPosition(relPos);
        SetDirty(DirtyCauses::Matrix);
    }

    void Node::SetDirection(const glm::vec3 direction) {
        Object3d::SetDirection(direction);
        SetDirty(DirtyCauses::Matrix);
    }

    void Node::SetRight(const glm::vec3 right) {
        Object3d::SetRight(right);
        SetDirty(DirtyCauses::Matrix);
    }

    void Node::SetUp(const glm::vec3 up) {
        Object3d::SetUp(up);
        SetDirty(DirtyCauses::Matrix);
    }

    void Node::SetLocalTransform(const glm::mat4 &Transform) {
        Object3d::SetLocalTransform(Transform);
        SetDirty(DirtyCauses::Matrix);
    }

    void Node::fire(const event::Node &evnt) const {
        // Observable::fire(evnt);
        if (scene_ != nullptr) {
            scene_->Handle(evnt);
        }
    }

    void Node::Update() {
        if (!IsDirty()) {
            return;
        }
        UpdateDirty();
    }

    void Node::UpdateDirty() {
        if (IsDirtyCause(DirtyCauses::Matrix)) {
            if (!isScene()) {
                ComputeWorldTransform(GetParent()->GetWorldTransform());
            }
            for (auto &compsOfType : components_) {
                for (auto *comp : compsOfType) {
                    comp->UpdateMatrix(GetWorldTransform());
                }
            }
        }
        for (auto &compsOfType : components_) {
            for (auto *comp : compsOfType) {
                comp->Update();
            }
        }
        for (auto *child : children_) {
            if (IsDirtyCause(DirtyCauses::Matrix)) {
                child->dirtyCause_[static_cast<int>(DirtyCauses::Matrix)] = true;
            }
            child->UpdateDirty();
        }
        dirtyCause_ = {0};
        SetState(State::Normal);
    }

    bool Node::IsDirty() const { return state_ == State::Dirty; }

    bool Node::IsDirtyCause(DirtyCauses cause) const { return dirtyCause_[static_cast<int>(cause)]; }

    Node::State Node::GetState() const { return state_; }

    bool Node::IsState(const State state) const { return state_ == state; }

    void Node::SetState(const State state) {
        if (state_ == state) {
            return;
        }
        state_ = state;
        fire(event::Node(this, event::Node::ChangeType::State));
    }

    void Node::SetDirty(DirtyCauses cause) {
        dirtyCause_[static_cast<int>(cause)] = true;
        SetState(State::Dirty);
    }

    Node::Type Node::GetType() const { return type_; }

    Node *Node::GetParent() const { return parent_; }

    const std::vector<Node *> &Node::GetChildren() const { return children_; }

    void Node::addComponent(component::Component *comp) {
        if (this == comp->GetParent()) {
            return;
        }
        comp->SetParent(this);
        if (components_.size() <= static_cast<int>(comp->GetType())) {
            components_.resize(static_cast<int>(comp->GetType()) + 1);
        }
        components_[static_cast<int>(comp->GetType())].push_back(comp);

        const auto evnt = event::Component(comp, event::Component::ChangeType::Added);
        if (scene_ != nullptr) {
            scene_->Handle(evnt);
        }
        SetDirty(DirtyCauses::Component);
    }

    void Node::RemoveComponent(component::Component *comp) {
        if (this != comp->GetParent()) {
            return;
        }
        if (components_.size() <= static_cast<int>(comp->GetType())) {
            return;
        }
        auto &compsOfType = components_[static_cast<int>(comp->GetType())];
        for (auto itr = compsOfType.begin(); itr != compsOfType.end(); ++itr) {
            if (comp == *itr) {
                compsOfType.erase(itr);
                comp->SetParent(nullptr);
                break;
            }
        }
        const auto evnt = event::Component(comp, event::Component::ChangeType::Removed);
        if (scene_ != nullptr) {
            scene_->Handle(evnt);
        }
    }

    void Node::Handle(const event::Component &event) {
        if (event.GetChangeType() == event::Component::ChangeType::State && event.GetOrigin()->IsDirty()) {
            SetDirty(DirtyCauses::Component);
        }
        if (scene_ != nullptr && !isScene()) {
            scene_->Handle(event);
        }
    }

    std::bitset<4> Node::GetReceiveTypes() const { return receiveTypeFlags_; }

    component::Component *Node::GetComponent(const component::Component::Type type) const {
        const auto &compsOfType = GetComponents(type);
        if (compsOfType.empty()) {
            return nullptr;
        }
        return compsOfType[0];
    }

    std::vector<component::Component *> Node::GetComponents(const component::Component::Type type) const {
        if (components_.size() <= static_cast<int>(type)) {
            return {};
        }
        return components_[static_cast<int>(type)];
    }
} // namespace soil::stage::scene
