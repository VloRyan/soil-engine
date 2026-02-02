#ifndef SOIL_STAGE_SCENE_GUI_RECTANGLE_H
#define SOIL_STAGE_SCENE_GUI_RECTANGLE_H

#include "stage/scene/gui/layout/anchor.h"
#include "stage/scene/node.h"
#include "video/render/state.h"

namespace soil::stage::scene::gui {

class Rectangle : public Node {
  friend class Root;

 public:
  enum class SizeTypes : std::uint8_t {
    Fixed = 0,
    Relative,
    GrowWithContent,
  };

  static inline auto LAYER_Z_INCREMENT = 0.1F;
  static inline auto LAYER_Z_COMPONENT_INCREMENT = 0.01F;
  static inline auto TOP_Z_LAYER = 100.0F;

  explicit Rectangle(const glm::ivec2& size = glm::ivec2(0),
                     SizeTypes sizeType = SizeTypes::Fixed);
  explicit Rectangle(SizeTypes sizeType);
  ~Rectangle() override = default;

  [[nodiscard]] virtual Rectangle* GetParentRect() const;

  virtual void SetSize(const glm::ivec2& size);
  [[nodiscard]] virtual const glm::ivec2& GetSize() const;

  [[nodiscard]] virtual const glm::vec2& GetRelativeSize() const;
  virtual void SetRelativeSize(const glm::vec2& relativeSize);

  void SetAnchor(const layout::Alignment& alignment);

  [[nodiscard]] virtual float GetAspectRatio() const;
  virtual void SetAspectRatio(float aspectRatio);

  [[nodiscard]] virtual bool IsMouseOver() const;

  [[nodiscard]] bool Contains(glm::ivec2 pos) const;
  [[nodiscard]] bool Contains(const Rectangle* other) const;

  [[nodiscard]] virtual glm::ivec2 GetMinSize() const;
  virtual void SetMinSize(const glm::ivec2& minSize);

  [[nodiscard]] virtual glm::ivec2 GetMaxSize() const;
  virtual void SetMaxSize(const glm::ivec2& maxSize);

  virtual void SetVisible(bool visible);
  virtual bool IsVisible() const;

  virtual void SetOnMouseOverFunc(
      const std::function<void(const glm::ivec2& pos)>& onMouseOverFunc);

  virtual void SetOnMouseOutFunc(const std::function<void()>& onMouseOutFunc);

  [[nodiscard]] virtual const video::render::Rect& GetScissorRect() const;

  virtual glm::ivec2 CalculateSize(const glm::ivec2& maxSize);

  [[nodiscard]] virtual const glm::ivec4& GetPadding() const;
  virtual void SetPadding(const glm::ivec4& padding);

  class Root* GuiRoot() const;

  virtual void FindChildrenAt(std::vector<Rectangle*>& result, glm::ivec2 pos,
                              bool onlyVisible);
  SizeTypes GetSizeType() const;
  void SetSizeType(SizeTypes sizeType);

  const glm::ivec2& GetChildrenSize() const;
  glm::ivec2 Paddings() const;

 protected:

  virtual void addChildRect(Rectangle* rect);
  //virtual void removeChildRect(const Rectangle* rect);

  void UpdateDirty() override;
  virtual void Layout();

  virtual void BeforeNodeUpdate() {};

  virtual void AfterNodeUpdate() {};

  virtual void UpdateScissor(const video::render::Rect& parentRect);

  virtual void UpdateVisibility(bool parentVisible);
  virtual void UpdateSize(const glm::ivec2& maxSize);
  virtual void UpdateChildrenSize(const glm::ivec2& maxSize);

  void ApplyAnchors();

  virtual void OnMouseOver(const glm::ivec2& pos);
  virtual void OnMouseOut();
  virtual void OnMouseButton(const glm::ivec2& pos, input::MouseButton button,
                             input::Event::StateType state);

  virtual void OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset);
  void OnChildRemoved(Node* node) override;
  void OnChildAdded(Node* node) override;
  virtual glm::ivec2 CalculateChildrenSize(const glm::ivec2& maxSize);
  virtual video::render::Rect CalculateChildScissorRect() const;

  glm::ivec2 CalculateMaxChildrenSize() const;

  virtual glm::ivec2 CalculateAlignedChildrenSize(const glm::ivec2& maxSize) const;
  virtual void updateChildrenSize(const glm::ivec2& maxSize, std::vector<Rectangle*>& children);

  std::vector<Rectangle*> childRects_;
  std::vector<Rectangle*> addedRects_;
  bool isMouseOver_;

  glm::ivec2 size_;
  SizeTypes sizeType_;
  glm::ivec2 childrenSize_;
  glm::ivec2 minSize_;
  glm::ivec2 maxSize_;
  glm::vec2 relativeSize_;
  float aspectRatio_;
  video::render::Rect scissorRect_;

  glm::ivec4 padding_;

  layout::Anchor anchor_;
  bool visible_;
  bool visibleEffective_;

  std::function<void(const glm::ivec2& pos)> onMouseOverFunc_;
  std::function<void()> onMouseOutFunc_;
};
}  // namespace soil::stage::scene::gui

#endif
