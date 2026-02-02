#ifndef SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_ALIGNMENT_HPP
#define SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_ALIGNMENT_HPP
namespace soil::stage::scene::gui::layout {
struct Alignment {

  enum class Horizontal { None = 0, Left, Center, Right };
  enum class Vertical { None = 0, Top, Center, Bottom };

  Horizontal X{Horizontal::None};
  Vertical Y{Vertical::None};

  bool operator==(const Alignment& rhs) const {
    return X == rhs.X && Y == rhs.Y;
  }

  bool operator!=(const Alignment& rhs) const { return !(rhs == *this); }

};
}
#endif
