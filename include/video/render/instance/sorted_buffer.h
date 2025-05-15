
#ifndef SOIL_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
#define SOIL_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
#include "buffer.h"

namespace soil::video::render::instance {
    class SortedBuffer final : public Buffer {
    public:
        enum class SortType {
            BackToFront,
            FrontToBack,
        };

        struct {
            bool operator()(const std::pair<float, Instance *> &a, const std::pair<float, Instance *> &b) const {
                if (a.first != b.first) {
                    return a.first < b.first;
                }
                return a.second < b.second;
            }
        } SortByDistanceBackToFront;

        struct {
            bool operator()(const std::pair<float, Instance *> &a, const std::pair<float, Instance *> &b) const {
                if (a.first != b.first) {
                    return a.first > b.first;
                }
                return a.second < b.second;
            }
        } SortByDistanceFrontToBack;

        SortedBuffer(buffer::Object *buffer, size_t instanceSize, SortType sortType);

        void Update(const glm::vec3 &viewerPos) override;
        [[nodiscard]] glm::vec3 GetRefPos() const;
        void SetRefPos(const glm::vec3 &ref_pos);

    private:
        SortType sortType_;
        glm::vec3 refPos;
    };
} // namespace soil::video::render::instance


#endif // SOIL_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
