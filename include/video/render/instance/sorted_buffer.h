
#ifndef ENGINE_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
#define ENGINE_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
#include "buffer.h"


namespace video::render::instance {
    class SortedBuffer : public Buffer {
    public:
        enum class SortType {
            BackToFront,
            FrontToBack,
        };

        struct {
            bool operator()(std::pair<float, Instance *> &a, std::pair<float, Instance *> &b) const {
                if (a.first != b.first) { return a.first < b.first; }
                return a.second < b.second;
            }
        } SortByDistanceBackToFront;

        struct {
            bool operator()(std::pair<float, Instance *> &a, std::pair<float, Instance *> &b) const {
                if (a.first != b.first) { return a.first > b.first; }
                return a.second < b.second;
            }
        } SortByDistanceFrontToBack;

        SortedBuffer(buffer::Object *buffer, size_t instanceSize, SortType sortType);

        void Update(const glm::vec3 &viewerPos) override;

    private:
        SortType sortType_;
    };
}


#endif //ENGINE_VIDEO_RENDER_INSTANCE_SORTED_BUFFER_H
