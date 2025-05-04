#ifndef ENGINE_VIDEO_MODEL_ANIMATION_H_
#define ENGINE_VIDEO_MODEL_ANIMATION_H_

#include <string>
#include <vector>
#include "animation_node.h"

namespace video::model {
    class Animation {
    public:
        enum class PlayMode {
            None = 0,
            Once,
            Repeat
        };

        enum class PlayDirection {
            Forward = 0,
            Backward
        };

        explicit Animation(std::string name);

        virtual ~Animation();

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] double getTicksPerSecond() const;

        [[nodiscard]] double getDuration() const;

        [[nodiscard]] std::vector<AnimationNode *> getChannels() const;

        [[nodiscard]] uint getNumChannels() const;

        void setTicksPerSecond(double TicksPerSecond);

        void setDuration(double Duration);

        [[nodiscard]] const AnimationNode *findNode(const std::string &name) const;

    private:
        /** The name of the animation. If the modeling package this data was   exported from does support only a single animation channel, this  name is usually empty (length is zero). */
        std::string name_;
        /** The scene animation channels. Each channel affects a single scene.  The array is mNumChannels in size. */
        std::vector<AnimationNode *> channels_;
        /** Duration of the animation in ticks.  */
        double duration_;
        /** Ticks per second. 0 if not specified in the imported file */
        double ticksPerSecond_;
    };
} // model

#endif //ENGINE_VIDEO_MODEL_ANIMATION_H_
