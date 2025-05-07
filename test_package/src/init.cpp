#include "engine.h"

int main() {
    constexpr auto winParams = soil::WindowParameter();
    const auto e = soil::Engine(winParams);
    e.Stop();
}
