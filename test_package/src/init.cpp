#include "engine.h"

int main() {
  constexpr auto args = soil::Engine::Args_t();
  const auto e = soil::Engine(args);
  e.Stop();
}
