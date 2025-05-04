#include "frame_limiter.h"
#include <plog/Log.h>

namespace engine
{
    FrameLimiter::FrameLimiter(const int FPSLimit, const CurrentTimeFun currentTime) :
        currentTimeFun_(currentTime), frameCounter_(0), framesPerSec_(0), fpsLimit(0), skippedFrames_(0),
        durationLastFrame_(0), startTimeSec_(0), startTimeCurrentFrame_(0), startTimeNextFrame_(0),
        startTimeLastFrame_(0), endTimeLastFrame_(0), linearTimePerFrame_(0)
    {
        SetFPSLimit(FPSLimit);
    }

    void FrameLimiter::WaitForFrame()
    {
        if (fpsLimit == 0 || startTimeNextFrame_ == 0)
        {
            /*Times has come*/
            startTimeCurrentFrame_ = currentTimeFun_();
            return;
        }
        while (startTimeNextFrame_ - (startTimeCurrentFrame_ = currentTimeFun_()) > 0.0)
        {
            // Do nothing
        }
    }

    void FrameLimiter::EndFrame()
    {
        ++frameCounter_;
        endTimeLastFrame_ = currentTimeFun_();
        if (endTimeLastFrame_ - startTimeSec_ >= 1.0)
        {
            // If last was more than 1sec ago
            framesPerSec_ = frameCounter_;
            frameCounter_ = 0;
            startTimeSec_ = endTimeLastFrame_;
        }
        // durationLastFrame_ = endTimeLastFrame_ - startTimeCurrentFrame_;
        startTimeNextFrame_ = startTimeSec_ + linearTimePerFrame_ * frameCounter_;
        startTimeLastFrame_ = startTimeCurrentFrame_;
        startTimeCurrentFrame_ = 0;
    }

    auto FrameLimiter::GetDeltaLastFrame() const -> double
    {
        if (startTimeLastFrame_ == 0.0)
        {
            return 0.0;
        }
        return startTimeCurrentFrame_ - startTimeLastFrame_;
    }

    void FrameLimiter::SetFPSLimit(const int limitFramesPerSec)
    {
        if (limitFramesPerSec < 0)
        {
            fpsLimit = 0;
            PLOG_DEBUG << "Setting limit fps to unlimited";
        }
        else if (limitFramesPerSec == 0)
        {
            fpsLimit = LimitFPSDefault - 1;
        }
        else
        {
            fpsLimit = limitFramesPerSec - 1;
        }
        PLOG_DEBUG << "Setting limit fps to " << (fpsLimit + 1);
        linearTimePerFrame_ = 1.0F / static_cast<float>(fpsLimit);
        /*Start new second*/
        startTimeSec_ = 0;
    }

    auto FrameLimiter::GetCurrentFPS() const -> int { return framesPerSec_; }

    auto FrameLimiter::GetFPSLimit() const -> int { return fpsLimit; }

    auto FrameLimiter::IsNewFps() const -> bool { return frameCounter_ == 0; }
} // namespace engine
