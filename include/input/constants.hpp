#ifndef INPUT_CONSTANTS_HPP
#define INPUT_CONSTANTS_HPP
#include <cstdint>

namespace soil::input {
enum class Keys : uint8_t {
  Unknown = 0,
  Key_0 = 48,
  Key_1 = 49,
  Key_2 = 50,
  Key_3 = 51,
  Key_4 = 52,
  Key_5 = 53,
  Key_6 = 54,
  Key_7 = 55,
  Key_8 = 56,
  Key_9 = 57,
  A = 65,
  B = 66,
  C = 67,
  D = 68,
  E = 69,
  F = 70,
  G = 71,
  H = 72,
  I = 73,
  J = 74,
  K = 75,
  L = 76,
  M = 77,
  N = 78,
  O = 79,
  P = 80,
  Q = 81,
  R = 82,
  S = 83,
  T = 84,
  U = 85,
  V = 86,
  W = 87,
  X = 88,
  Y = 89,
  Z = 90,

  Cursor_Left,
  Cursor_Right,
  Cursor_Up,
  Cursor_Down,
  Escape,
  Enter,
  Backspace,
  Page_Up,
  Page_Down,
  Delete,
  Pos1,
  End,
  Insert,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  Key_Plus,
  Key_Minus,
  Key_Space,
  Key_Left_Shift,
  /** 94 -> '^'*/
  Circumflex,
  Tab
};

enum class MouseButton : uint8_t {
  Left = 0,
  Right,
  Middle,
  Mouse_4,
  Mouse_5,
  Mouse_6,
  Mouse_7,
  Mouse_8
};

enum class MouseWheelDirection : uint8_t { Up = 0, Down };
}  // namespace soil::input
#endif  // INPUT_CONSTANTS_HPP
