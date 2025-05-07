
#ifndef SOIL_VIDEO_SHADER_STAGE_H
#define SOIL_VIDEO_SHADER_STAGE_H
#include <list>
#include <string>

#define GLSL_SHADER_FILE_EXTENSION ".glsl"
#define GLSL_SHADER_FILE_VERTEX "vert"
#define GLSL_SHADER_FILE_FRAGMENT "frag"
#define GLSL_SHADER_FILE_GEOMETRY "geom"
#define GLSL_SHADER_FILE_TESS_CONTROL "tesc"
#define GLSL_SHADER_FILE_TESS_EVALUATION "tese"
#define GLSL_SHADER_FILE_COMPUTE "comp"

namespace soil::video::shader {
    /**
     * Stage of a shader program. E.g. vertex, fragment etc.
     */
    class Stage final {
    public:
        ~Stage();

        static Stage *load(std::string fileName, uint type);

        static void PrintShaderInfoLog(uint shader);

        void AttachToProgram(uint programId);

        [[nodiscard]] uint GetId() const;

    protected:
        Stage(uint id, uint type);

        static size_t replaceLine(const std::string &line, std::list<std::string> &lineBuffer,
                                  std::string &originFileName);

    private:
        static Stage *compile(const char *source, uint type);

    protected:
        uint id_;
        uint programId_;
        uint type_;
    };
} // namespace soil::video::shader
#endif // SOIL_VIDEO_SHADER_STAGE_H
