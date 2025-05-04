#ifndef ENGINE_VIDEO_MODEL_ANIMATION_NODE_H_
#define ENGINE_VIDEO_MODEL_ANIMATION_NODE_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

#include "base.h"

namespace video::model {
    class AnimationNode {
    public:
        struct VectorKey {
            /** The time of this key */
            double Time;

            /** The value of this key */
            glm::vec3 Value;

            VectorKey(const double time, const glm::vec3 value) : Time(time), Value(value) {
            }
        };

        struct QuatKey {
            /** The time of this key */
            double Time;

            /** The value of this key */
            glm::quat Value;

            QuatKey(const double time, const glm::quat value)
                : Time(time), Value(value) {
            }
        };

        explicit AnimationNode(std::string name);

        virtual ~AnimationNode();

        [[nodiscard]] std::vector<VectorKey> getScalingKeys() const;

        [[nodiscard]] uint getNumScalingKeys() const;

        [[nodiscard]] std::vector<QuatKey> getRotationKeys() const;

        [[nodiscard]] uint getNumRotationKeys() const;

        [[nodiscard]] std::vector<VectorKey> getPositionKeys() const;

        [[nodiscard]] uint getNumPositionKeys() const;

        [[nodiscard]] std::string getNodeName() const;

        void CalcInterpolatedScaling(glm::vec3 &Out, float AnimationTime) const;

        void CalcInterpolatedRotation(glm::quat &Out, float AnimationTime) const;

        void CalcInterpolatedPosition(glm::vec3 &Out, float AnimationTime) const;

    protected:
        [[nodiscard]] uint FindScaling(float AnimationTime) const;

        [[nodiscard]] uint FindRotation(float AnimationTime) const;

        [[nodiscard]] uint FindPosition(float AnimationTime) const;

    private:
        /** The name of the scene affected by this animation. The scene
         *  must exist and it must be unique.*/
        std::string nodeName_;

        /** The position keys of this animation channel. Positions are
         * specified as 3D vector. The array is mNumPositionKeys in size.
         *
         * If there are position keys, there will also be at least one
         * scaling and one rotation key.*/
        std::vector<VectorKey> positionKeys_;

        /** The rotation keys of this animation channel. Rotations are
         *  given as quaternions,  which are 4D vectors. The array is
         *  mNumRotationKeys in size.
         *
         * If there are rotation keys, there will also be at least one
         * scaling and one position key. */
        std::vector<QuatKey> rotationKeys_;

        /** The scaling keys of this animation channel. Scalings are
         *  specified as 3D vector. The array is mNumScalingKeys in size.
         *
         * If there are scaling keys, there will also be at least one
         * position and one rotation key.*/
        std::vector<VectorKey> scalingKeys_;
    };
} // model

#endif //ENGINE_VIDEO_MODEL_ANIMATION_NODE_H_
