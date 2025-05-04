#ifndef ENGINE_ENGINE_FRAME_LIMITER_H_
#define ENGINE_ENGINE_FRAME_LIMITER_H_

#include "base.h"

namespace engine {
    //#define ENGINE_DEFAULT_LIMIT_FPS 120
    constexpr int LimitFPSDefault = 120;
    using CurrentTimeFun = double (*)();

    class FrameLimiter {
    public:
        explicit FrameLimiter(int FPSLimit, CurrentTimeFun currentTime);

        void WaitForFrame();

        void EndFrame();

        [[nodiscard]] auto GetDeltaLastFrame() const -> double;

        void SetFPSLimit(int limitFramesPerSec);

        [[nodiscard]] auto GetCurrentFPS() const -> int;

        [[nodiscard]] auto IsNewFps() const -> bool;

        [[nodiscard]] auto GetFPSLimit() const -> int;

    private:
        CurrentTimeFun currentTimeFun_;
        /** Counter for frames in one second*/
        int frameCounter_;
        /** frames in previous second*/
        int framesPerSec_;
        /** limit of frames in one second*/
        int fpsLimit;

        /*Skipped frames between the current frame and the previous. THIS IS A COUNTER AND THE VALUE IS VARIING*/
        uint skippedFrames_;
        /** Duration of the last rendered frame*/
        double durationLastFrame_;

        /** Start time of current second*/
        double startTimeSec_;
        /** Start time of current frame*/
        double startTimeCurrentFrame_;
        /** Times for the next frame in limitFramesPerSecond*/
        double startTimeNextFrame_;
        /** Start time of previous frame*/
        double startTimeLastFrame_;
        /** End time of previous frame*/
        double endTimeLastFrame_;
        /** Linear time between frames according to limitFramesPerSecond*/
        double linearTimePerFrame_;
    };
} // engine

#endif //ENGINE_ENGINE_FRAME_LIMITER_H_
