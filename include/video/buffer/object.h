#ifndef SOIL_VIDEO_BUFFER_OBJECT_H
#define SOIL_VIDEO_BUFFER_OBJECT_H
#include "base.h"
#include "cursor.h"

namespace soil::video::buffer {
    class Object {
    public:
        enum class Types : std::uint8_t { Vertex, ElementArray, Uniform, ShaderStorage, DrawIndirect };

        /**
         * Expected usage of the Object
         */
        enum class UsageType : std::uint8_t {
            /**The data store contents will be modified once and used many times.*/
            Static,
            /**The data store contents will be modified once and used at most a few times.*/
            Stream,
            /**The data store contents will be modified repeatedly and used many times.*/
            Dynamic
        };

        enum class AccessType : std::uint8_t {
            /**The data store contents are modified by the application, and used as the source for GL drawing and image
               specification commands.*/
            Draw,
            /**The data store contents are modified by reading data from the GL, and used to return that data when
               queried by the application.*/
            Read,
            /**The data store contents are modified by reading data from the GL, and used as the source for GL drawing
               and image specification commands.*/
            Copy
        };

        Object(const Object &orig) = delete;

        /** Default destructor */
        virtual ~Object();

        [[nodiscard]] uint GetId() const;

        [[nodiscard]] UsageType GetUsage() const;

        [[nodiscard]] bool IsUsage(UsageType desired) const;

        [[nodiscard]] uint GetBufferSize() const;

        virtual void SetData(const void *data, gl_size_t dataSize);

        [[nodiscard]] const void *GetData() const;

        virtual void Flush();

        virtual void Reserve(gl_size_t newBufferSize);

        [[nodiscard]] bool IsCreated() const;

        void Bind() const;

        void Unbind() const;

        void BindAs(Types type) const;

        void UnbindAs(Types type) const;

        void BindToTarget(uint index) const;

        void BindToTarget(uint index, Types target) const;

        [[nodiscard]] AccessType GetAccess() const;

        [[nodiscard]] Cursor *GetCursor();

    protected:
        virtual void create();

        virtual void uploadData(const void *data, gl_size_t dataSize);

        Object(Types bufferType, UsageType usage, AccessType access = AccessType::Draw);

        [[nodiscard]] Types getBufferType() const;

        [[nodiscard]] uint getGLBufferType(Types type) const;

        [[nodiscard]] uint getGLUsageType() const;


        /**Local buffer*/
        byte *data_;
        /**Cursor to current buffer(OpenGL)*/
        Cursor *cursor_;
        uint id_;
        /**Size of local or OpenGl Buffer*/
        gl_size_t bufferSize_;
        Types bufferType_;
        UsageType usage_;
        AccessType access_;
    };
} // namespace soil::video::buffer
#endif /* SOIL_VIDEO_BUFFER_OBJECT_H */
