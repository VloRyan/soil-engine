#ifndef SOIL_VIDEO_SHADER_DEFINITION_H
#define SOIL_VIDEO_SHADER_DEFINITION_H
#include <string>

namespace soil::video::shader {
    class Definition final {
    public:
        Definition();

        Definition(Definition &other);

        Definition(std::string vertexShaderName, std::string fragmentShaderName);

        ~Definition();

        void SetFragmentShaderName(std::string FragmentShaderName);

        [[nodiscard]] std::string GetFragmentShaderName() const;

        [[nodiscard]] std::string GetFragmentShaderFilePath() const;

        void SetTessellationEvalStageShaderName(std::string TessellationEvalStageShaderName);

        [[nodiscard]] std::string GetTessellationEvalStageShaderName() const;

        [[nodiscard]] std::string GetTessellationEvalStageShaderFilePath() const;

        void SetTessellationControlShaderName(const std::string &TessellationControlShaderName);

        [[nodiscard]] std::string GetTessellationControlShaderName() const;

        [[nodiscard]] std::string GetTessellationControlShaderFilePath() const;

        void SetGeometryShaderName(const std::string &GeometryShaderName);

        [[nodiscard]] std::string GetGeometryShaderName() const;

        [[nodiscard]] std::string GetGeometryShaderFilePath() const;

        void SetVertexShaderName(const std::string &VertexShaderName);

        [[nodiscard]] std::string GetVertexShaderName() const;

        [[nodiscard]] std::string GetVertexShaderFilePath() const;

        void SetComputeShaderName(const std::string &ComputeShaderName);

        [[nodiscard]] std::string GetComputeShaderName() const;

        [[nodiscard]] std::string GetComputeShaderFilePath() const;

        void SetFilePath(const std::string &filePath);

        [[nodiscard]] std::string GetFilePath() const;

        [[nodiscard]] std::string ComputeName() const;

    private:
        std::string filePath_;
        std::string computeShaderName_;
        std::string vertexShaderName_;
        std::string fragmentShaderName_;
        std::string geometryShaderName_;
        std::string tessellationControlShaderName_;
        std::string tessellationEvalShaderName_;
    };
} // namespace soil::video::shader
#endif
