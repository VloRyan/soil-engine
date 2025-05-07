#ifndef SOIL_ID_H_
#define SOIL_ID_H_

#include <atomic>
#include "base.h"

namespace soil {
    class Id {
    public:
        static auto Next() -> uint;

    private:
        static std::atomic_uint currentId;
    };
} // engine

#endif //SOIL_ID_H_
