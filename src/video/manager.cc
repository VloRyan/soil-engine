#include "video/manager.h"
#include <GL/gl3w.h>
#include <plog/Log.h>
#include <vector>
#include "util/strings.h"
#include "video/adapter.h"
#include "video/mesh/cache.h"
#include "video/shader/cache.h"
#include "video/texture/manager.h"

#include "stage/scene/component/aabb.h"
#include "video/buffer/frame_buffer.h"
#include "video/render/state.h"
#include "window.h"

namespace video
{
    Manager::Manager() : window_(nullptr), adapter(OTHER, ""), nextMeshId(1) {}

    Manager::~Manager() {}

    void Manager::Init(engine::Window *window)
    {
        window_ = window;
        if (!window_->HasState(engine::WindowState::Open))
        {
            window_->Open();
        }
        const int retVal = gl3wInit();
        if (retVal != GL3W_OK)
        {
            throw engine::Exception("failed to initialize GL3W with code " + std::to_string(retVal));
        }
        if (gl3wIsSupported(OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION) == 0)
        {
            throw engine::Exception("OpenGL " + std::to_string(OPEN_GL_MAJOR_VERSION) + "." +
                                    std::to_string(OPEN_GL_MINOR_VERSION) + " not supported.");
        }
#ifdef DEBUG
        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(Manager::debugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else
        {
            PLOG_ERROR << "OPENGL_DEBUG_CONTEXT failed";
        }
#endif
        PLOG_DEBUG.printf("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
        Vendor vendor = Vendor::OTHER;
        const std::string vendorStr((const char *)glGetString(GL_VENDOR));
        if (util::Strings::startsWith(vendorStr, "ATI"))
        {
            vendor = Vendor::ATI;
        }
        if (util::Strings::startsWith(vendorStr, "NVIDIA"))
        {
            vendor = Vendor::NVIDIA;
        }
        if (util::Strings::startsWith(vendorStr, "INTEL"))
        {
            vendor = Vendor::INTEL;
        }
        adapter = Adapter(vendor, (const char *)glGetString(GL_RENDERER));
        PLOG_DEBUG.printf("Vendor %s: %s", vendorStr.c_str(), adapter.GetModel().c_str());
#ifdef DEBUG
        /*int numExtensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        for (uint i = 0; i < numExtensions; i++) {
            PLOG_DEBUG.printf("Extension(%d): %s", i, glGetStringi(GL_EXTENSIONS, i));
        }*/
#endif
        initState();
    }

    void Manager::Update() { textureManager_.Update(); }

    void Manager::BeginRender()
    {
        GetState().SetFramebuffer(nullptr);
        GetState().SetViewPort(window_->GetSize());
        glClear(COLOR_AND_DEPTH_BUFFER_BIT);
    }

    void Manager::EndRender() const { window_->SwapBuffers(); }

    void Manager::NewUniformBufferObject(const std::string &name, const gl_size_t size, const int target)
    {
        auto *ubo = new buffer::UniformBufferObject(buffer::Object::UsageType::Dynamic);
        ubo->Reserve(size);
        ubo->BindToTarget(target);
        shaderCache_.ForEach([name, target](shader::Shader *shader) { shader->BindUniformBlock(name, target); });
        glBindBufferBase(GL_UNIFORM_BUFFER, target, ubo->GetId());
        state_.RegisterUbo(target, ubo);
    }

    void Manager::renderToScreen(const std::vector<texture::Texture *> &textures)
    {
        // START_MEASURE_TIME("renderToScreen State");
        state_.SetDepthTest(true);
        state_.SetBlend(true);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, static_cast<GLsizei>(window_->GetSize().x), static_cast<GLsizei>(window_->GetSize().y));
        // STOP_MEASURE_TIME("renderToScreen State");

        // START_MEASURE_TIME("renderToScreen textureShader");
        auto *textureShader = GetShader(shader::COLOR_CORRECTION);
        textureShader->Use();
        // STOP_MEASURE_TIME("renderToScreen textureShader");

        // START_MEASURE_TIME("renderToScreen drawTriangles");
        /*   quadObject_->GetVao()->Bind();
           for (const auto *texture: textures) {
               textureShader->SetTexture2d(0, texture, "Texture");
               shader::Shader::DrawElements(GL_TRIANGLES,
           quadObject_->GetIndexCount(), quadObject_->GetIndexType());
           }*/
        // STOP_MEASURE_TIME("renderToScreen drawTriangles");
    }

    void Manager::initState()
    {
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);

        glClearColor(0.1F, 0.1F, 0.1F, 0.0f);
        glClearDepth(1.0F);

        // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        // glEnable(GL_MULTISAMPLE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        state_.Init();
        state_.SetDepthTest(false);
        state_.SetBlend(false);
        state_.SetStencilTest(false);
    }

    mesh::Mesh *Manager::GetMesh(const mesh::Definition &definition)
    {
        return meshCache_.GetOrPut(definition, [definition, this](const std::string &name)
                                   { return mesh::Mesh::New(nextMeshId++, definition.Identifier, definition.Type); });
    }

    shader::Shader *Manager::GetShader(const std::string &name) { return shaderCache_.GetByName(name); }

    shader::InstanceShader *Manager::GetIShader(const std::string &name) { return shaderCache_.GetByName2(name); }

    void Manager::PrepareShader(const std::string &name, const std::string &path) { shaderCache_.Prepare(name, path); }

    void Manager::PrepareShader(shader::Shader *shader) { shaderCache_.Prepare(shader->GetName(), shader); }

    void Manager::PrepareIShader(shader::InstanceShader *shader)
    {
        shaderCache_.PrepareInstanceShader(shader->GetName(), shader);
    }

    texture::Manager &Manager::Texture() { return textureManager_; }

    render::State &Manager::GetState() { return state_; }

    void APIENTRY Manager::debugOutput(const GLenum source, const GLenum type, const GLuint id, const GLenum severity,
                                       const GLsizei length, const GLchar *message, const void *userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
        /* Pixel-path performance warning: Pixel transfer is synchronized with 3D
         * rendering.*/
        if (id == 131154) return;
        std::string debugMessage = "ID(" + std::to_string(id) + ") Source(";

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            debugMessage += "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            debugMessage += "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            debugMessage += "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            debugMessage += "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            debugMessage += "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
        default:
            debugMessage += "Other";
            break;
        }
        debugMessage += ") Type(";

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            debugMessage += "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            debugMessage += "Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            debugMessage += "Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            debugMessage += "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            debugMessage += "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            debugMessage += "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            debugMessage += "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            debugMessage += "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
        default:
            debugMessage += "Other";
            break;
        }
        debugMessage += ") Severity(";

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            debugMessage += "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            debugMessage += "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            debugMessage += "low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            debugMessage += "notification";
            break;
        default:
            debugMessage += "other";
            break;
        }

        if (length > 0)
        {
            debugMessage += ")\t";
            debugMessage.append(message, static_cast<uint>(length));
        }
        else
        {
            debugMessage += ")";
        }
        PLOG_DEBUG << debugMessage;
    }
} // namespace video
