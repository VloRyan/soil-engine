
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

    private:
        SortType sortType_;
    };
} // namespace soil::video::render::instance


#endif // SOIL_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
