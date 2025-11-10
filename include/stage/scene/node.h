#ifndef SOIL_STAGE_SCENE_NODE_H
#define SOIL_STAGE_SCENE_NODE_H

#include <bitset>
#include <cstdint>
#include <functional>
#include <vector>

#include "component/component.h"
#include "event/observable.hpp"
#include "input/event.h"
#include "stage/event/component.h"
#include "stage/event/node.h"
#include "stage/scene/component/transform_component.h"
#include "video/render/state.h"
#include "window_event.h"
#include "world/entity/object_3d.h"
namespace soil::stage {
class Stage;
}
namespace soil::stage::scene {
class Scene;

class Node : public input::EventHandler,
             public WindowEventHandler,
             public event::ComponentEventHandler,
             public soil::event::Observable<event::Node> {
 public:
  enum class State : std::uint8_t {
    Normal = 0,
    Dirty,
    Delete,
  };

  enum class UpdateType : std::uint8_t {
    Active,
    Passive,
  };

  enum class DirtyImpact : std::uint8_t {
    /** Updates only itself*/
    Self = 0,
    /** Like Self + all components*/
    Components,
    /** Like Components + all children, also transitive children*/
    Dependents,
    /** Like Dependents + recalculate WorldTransform for every dependent node
       and update matrix of all dependent components*/
    Transform,
  };

  enum class Type : std::uint8_t {
    Scene = 0,
    Viewer,
    Visual,
    Game,
    Sound,
    Transform,
    Input,
    World,
  };

  enum class ReceiverType : std::uint8_t { None = 0, Window, Input, COUNT };

  explicit Node(Type type);

  ~Node() override;

  [[nodiscard]] bool IsDirty() const;

  [[nodiscard]] State GetState() const;

  virtual void SetState(State state);

  virtual void SetDirty(DirtyImpact cause);

  [[nodiscard]] bool IsState(State state) const;

  [[nodiscard]] bool IsDirtyImpact(DirtyImpact cause) const;

  [[nodiscard]] Node* GetParent() const;

  [[nodiscard]] const std::vector<Node*>& GetChildren() const;

  void ForEachComponent(const std::function<void(component::Component*)>& func,
                        component::Component::Type ofType =
                            component::Component::Type::Any) const;

  void GetComponents(
      std::vector<component::Component*>& comps,
      component::Component::Type type = component::Component::Type::Any) const;

  component::Component* GetFirstComponent(
      component::Component::Type type = component::Component::Type::Any) const;

  template <class T>
  T AddComponent(T comp) {
    using type = std::remove_pointer_t<T>;
    static_assert(std::is_base_of_v<component::Component, type>,
                  "comp must be derived from component::Component");
    addComponent(comp);
    return comp;
  }

  void RemoveComponent(component::Component* comp);

  [[nodiscard]] bool HasComponent(component::Component::Type type) const;

  [[nodiscard]] bool IsReceiverOf(ReceiverType type) const;

  virtual void Update();

  void Handle(const event::Component& event) override;

  [[nodiscard]] UpdateType GetUpdateType() const;

  [[nodiscard]] virtual Type GetType() const;
  virtual glm::vec3 GetPosition() const;
  virtual void SetPosition(const glm::vec3& pos);

  glm::vec3 GetLocalPosition() const;
  void SetLocalPosition(const glm::vec3& pos);

  virtual void Render(video::render::State& state);

  // void SetDirection(const glm::vec3& direction) override;

  // void SetRight(const glm::vec3& right) override;

  // void SetUp(const glm::vec3& up) override;

  // void SetTransform(const glm::mat4& transform) override;

  void Handle(const input::Event& event) override {}

  void Handle(const WindowEvent& event) override {}

  template <class T>
  T AddChild(T node) {
    using type = std::remove_pointer_t<T>;
    static_assert(std::is_base_of_v<Node, type>,
                  "node must be derived from scene::Node");
    addChild(node);
    return node;
  }

  static void ForEachChild(const Node* node,
                           const std::function<void(Node* child)>& func);

  virtual void RemoveChild(Node* node);

  component::TransformComponent& Transform() const;
  scene::Scene* Root() const;

 protected:
  void MarkDirtyWith(DirtyImpact cause);

  virtual void SetParent(Node* parent);

  virtual void UpdateDirty();

  virtual void SetUpdateType(UpdateType type);

  virtual void addChild(Node* node);

  void addComponent(component::Component* comp);

  static bool IsDirtyImpact(const std::bitset<4>& dirtyImpacts,
                            DirtyImpact impact);

  virtual void SetReceiverType(ReceiverType type, bool value);

  [[nodiscard]] virtual std::bitset<4> GetDirtyImpacts() const;

  virtual void OnStageChanged(Stage* stage, Stage* prevStage);

  void fire(const event::Node& event) override;

  component::TransformComponent* transform_;
  void incorporateAddedComponents();

 private:
  Type type_;
  Node* parent_;
  State state_;
  UpdateType updateType_;
  std::bitset<4> dirtyImpacts_;
  std::vector<Node*> children_;
  std::vector<component::Component*> addedComponents_;
  std::unordered_map<std::int8_t, std::vector<component::Component*>>
      components_;
  std::bitset<4> receiveTypeFlags_;
  std::vector<component::Component*> alwaysUpdateComponents_;
};
}  // namespace soil::stage::scene
#endif
