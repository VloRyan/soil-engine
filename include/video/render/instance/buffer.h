
#ifndef ENGINE_VIDEO_RENDER_INSTANCE_BUFFER_H
#define ENGINE_VIDEO_RENDER_INSTANCE_BUFFER_H
#include <vector>
#include "video/buffer/object.h"

#include "video/render/instance/instance.h"

namespace video::render::instance {
    class Buffer {
    public:
        Buffer(const Buffer &other) = delete;

        Buffer(Buffer &&other) noexcept = delete;

        Buffer &operator=(const Buffer &other) = delete;

        Buffer &operator=(Buffer &&other) noexcept = delete;

        Buffer(buffer::Object *buffer, size_t instanceSize);

        virtual ~Buffer();

        void AddChangedInstance(Instance *instance);

        void AddInstance(Instance *instance);

        bool RemoveInstance(Instance *instance);

        [[nodiscard]] buffer::Object* GetPerInstanceBuffer() const;

        [[nodiscard]] size_t GetInstanceSize() const;

        [[nodiscard]] size_t GetPreparedInstancesCount() const;

        virtual void Update(const glm::vec3 &viewerPos);

    protected:
        std::vector<Instance *> dirtyInstances_;
        std::vector<Instance *> preparedInstances_;

        buffer::Object *perInstanceBuffer_;
        size_t instanceSize_;
    };
}

#endif //ENGINE_VIDEO_RENDER_INSTANCE_BUFFER_H
