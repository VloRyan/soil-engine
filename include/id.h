#ifndef ENGINE_ID_H_
#define ENGINE_ID_H_

#include <atomic>
#include "base.h"

namespace engine {
    class Id {
    public:
        static auto Next() -> uint;

    private:
        static std::atomic_uint currentId;
    };
} // engine

#endif //ENGINE_ID_H_
