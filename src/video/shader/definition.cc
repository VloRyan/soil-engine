#include <utility>

#include "video/shader/definition.h"
#include "video/shader/stage.h"
#include "util/files.h"
#include "exception.h"

namespace video::shader {
    Definition::Definition() = default;

    Definition::Definition(std::string vertexShaderName, std::string fragmentShaderName)
        : vertexShaderName_(std::move(vertexShaderName)), fragmentShaderName_(std::move(fragmentShaderName)) {
    }

    Definition::Definition(Definition &other) = default;

    Definition::~Definition() = default;

    void Definition::SetFragmentShaderName(std::string FragmentShaderName) {
        fragmentShaderName_ = std::move(FragmentShaderName);
    }

    std::string Definition::GetFragmentShaderName() const {
        return fragmentShaderName_;
    }

    std::string Definition::GetFragmentShaderFilePath() const {
        return filePath_ + fragmentShaderName_;
    }

    void Definition::SetTessellationEvalStageShaderName(std::string TessellationEvalStageShaderName) {
        tessellationEvalShaderName_ = std::move(TessellationEvalStageShaderName);
    }

    std::string Definition::GetTessellationEvalStageShaderName() const {
        return tessellationEvalShaderName_;
    }

    std::string Definition::GetTessellationEvalStageShaderFilePath() const {
        return filePath_ + tessellationEvalShaderName_;
    }

    void Definition::SetTessellationControlShaderName(const std::string &TessellationControlShaderName) {
        tessellationControlShaderName_ = TessellationControlShaderName;
    }

    std::string Definition::GetTessellationControlShaderName() const {
        return tessellationControlShaderName_;
    }

    std::string Definition::GetTessellationControlShaderFilePath() const {
        return filePath_ + tessellationControlShaderName_;
    }

    void Definition::SetGeometryShaderName(const std::string &GeometryShaderName) {
        geometryShaderName_ = GeometryShaderName;
    }

    std::string Definition::GetGeometryShaderName() const {
        return geometryShaderName_;
    }

    std::string Definition::GetGeometryShaderFilePath() const {
        return filePath_ + geometryShaderName_;
    }

    void Definition::SetVertexShaderName(const std::string &VertexShaderName) {
        vertexShaderName_ = VertexShaderName;
    }

    std::string Definition::GetVertexShaderName() const {
        return vertexShaderName_;
    }

    std::string Definition::GetVertexShaderFilePath() const {
        return filePath_ + vertexShaderName_;
    }

    void Definition::SetComputeShaderName(const std::string &ComputeShaderName) {
        computeShaderName_ = ComputeShaderName;
    }

    std::string Definition::GetComputeShaderName() const {
        return computeShaderName_;
    }

    std::string Definition::GetComputeShaderFilePath() const {
        return filePath_ + computeShaderName_;
    }

    void Definition::SetFilePath(const std::string &filePath) {
        if (filePath.ends_with("/")) {
            filePath_ = filePath;
        } else {
            filePath_ = filePath + "/";
        }
    }

    std::string Definition::GetFilePath() const {
        return filePath_;
    }

    std::string Definition::ComputeName() const {
        std::string pipelineName;
        if (!computeShaderName_.empty()) {
            pipelineName += "_" + computeShaderName_;
        }
        if (!vertexShaderName_.empty()) {
            pipelineName += "_" + vertexShaderName_;
        }
        if (!fragmentShaderName_.empty()) {
            pipelineName += "_" + fragmentShaderName_;
        }
        if (!geometryShaderName_.empty()) {
            pipelineName += "_" + geometryShaderName_;
        }
        if (!tessellationControlShaderName_.empty()) {
            pipelineName += "_" + tessellationControlShaderName_;
        }
        if (!tessellationEvalShaderName_.empty()) {
            pipelineName += "_" + tessellationEvalShaderName_;
        }
        return pipelineName;
    }
}
