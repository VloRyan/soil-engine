
#ifndef SOIL_VIDEO_RENDER_INSTANCE_BUFFER_H
#define SOIL_VIDEO_RENDER_INSTANCE_BUFFER_H
#include <vector>
#include "video/buffer/object.h"

#include "video/render/instance/instance.h"

namespace soil::video::render::instance {
    class Buffer {
    public:
        Buffer(const Buffer &other) = delete;

        Buffer(Buffer &&other) noexcept = delete;

        Buffer &operator=(const Buffer &other) = delete;

        Buffer &operator=(Buffer &&other) noexcept = delete;

        Buffer(buffer::Object *buffer, size_t instanceSize);

        virtual ~Buffer();

        void AddChangedInstance(Instance *instance);

        void PrepareInstance(Instance *instance);

        bool RemoveInstance(Instance *instance);

        [[nodiscard]] buffer::Object *GetPerInstanceBuffer() const;

        [[nodiscard]] size_t GetInstanceSize() const;

        [[nodiscard]] size_t GetInstancesCount() const;
        [[nodiscard]] size_t GetDirtyInstancesCount() const;

        virtual void Update(const glm::vec3 &viewerPos);

        [[nodiscard]] virtual std::vector<Instance *> &GetInstances();

    protected:
        std::vector<Instance *> dirtyInstances_;
        std::vector<Instance *> instances_;

        buffer::Object *buffer_;
        size_t instanceSize_;
    };
} // namespace soil::video::render::instance

#endif // SOIL_VIDEO_RENDER_INSTANCE_BUFFER_H
