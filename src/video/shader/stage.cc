#include "video/shader/stage.h"
#include <algorithm>
#include <fstream>
#include "GL/gl3w.h"

#include "exception.h"
#include "plog/Log.h"
#include "util/files.h"

namespace soil::video::shader {
    Stage::Stage(const uint id, const uint type) : id_(id), programId_(0), type_(type) {}

    Stage::~Stage() {
        if (id_ > 0) {
            glDetachShader(programId_, id_);
            glDeleteShader(id_);
            id_ = 0;
        }
    }

    size_t Stage::replaceLine(const std::string &line, std::list<std::string> &lineBuffer,
                              std::string &originFileName) {
        if (line.starts_with("//#include ")) {
            std::string currentDir = util::Files::GetDirectory(originFileName);
            std::string includeFilename = line.substr(11);
            std::string includeFile = currentDir + "includes/" + includeFilename + ".glsl";
            std::ifstream in(includeFile);
            if (!in.is_open()) {
                std::string message = "Impossible to open the include file '" + includeFilename + "'";
                throw Exception(message);
            }
            size_t length = 0;
            std::string includeLine;
            while (std::getline(in, includeLine)) {
                if (includeLine.empty()) {
                    continue;
                }
                lineBuffer.push_back(includeLine);
                length += includeLine.length() + 1; // +1 line terminator
            }
            in.close();
            return length;
        }
        PLOG_ERROR.printf("Unknown directive: %s", line.c_str());
        lineBuffer.push_back(line);
        return line.length() + 1; // +1 line terminator
    }

    Stage *Stage::load(std::string fileName, uint type) {
        std::string line;
        size_t length = 0;
        std::list<std::string> lineBuffer;

        std::ifstream in(fileName);
        if (!in.is_open()) {
            std::string message = "Impossible to open the shader file '" + fileName + "'";
            throw Exception(message);
        }
        while (std::getline(in, line)) {
            if (line.empty()) {
                continue;
            }
            if (line.starts_with("//#include")) {
                // replacement
                length += replaceLine(line, lineBuffer, fileName);
            } else {
                lineBuffer.push_back(line);
                length += line.length() + 1; // +1 line terminator
            }
        }
        in.close();

        char buffer[length];
        uint offset = 0;
        for (auto elem : lineBuffer) {
            if (offset > 0) {
                buffer[offset++] = '\n';
            }
            std::ranges::copy(elem, &buffer[offset]);
            offset += elem.length();
        }
        buffer[offset] = '\0';
        Stage *stage = compile(buffer, type);
        if (stage == nullptr) {
            /* Return NULL on failure */
            std::string message = "Impossible to compile the shader file '" + fileName + "'";
            throw Exception(message);
        }
        return stage; /* Return the buffer */
    }

    void Stage::PrintShaderInfoLog(const uint shader) {
        int infoLogLen = 0;
        int charsWritten = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        if (infoLogLen > 0) {
            GLchar *infoLog = nullptr;
            infoLog = new GLchar[infoLogLen];
            // error check for fail to allocate memory omitted
            glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
            PLOG_INFO << infoLog;
            delete[] infoLog;
        }
    }

    Stage *Stage::compile(const char *source, const uint type) {
        /* Assign our handles a "name" to new shader objects */
        const uint shaderId = glCreateShader(type);
        /* Associate the source code buffers with each handle */
        glShaderSource(shaderId, 1, &source, nullptr);
        /* Compile our shader objects */
        glCompileShader(shaderId);
        GLint compiled;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
        std::string typeStr;
        if (compiled == GL_FALSE) {
            switch (type) {
            case GL_VERTEX_SHADER:
                typeStr = "Vertex";
                break;
            case GL_FRAGMENT_SHADER:
                typeStr = "Fragment";
                break;
            default:
                typeStr = "Type(" + std::to_string(type) + ")";
                break;
            }

            PLOG_ERROR.printf("%s shader not compiled.", typeStr.c_str());
            PrintShaderInfoLog(shaderId);
            glDeleteShader(shaderId);
            return nullptr;
        }
        return new Stage(shaderId, type);
    }

    void Stage::AttachToProgram(const uint programId) {
        programId_ = programId;
        glAttachShader(programId, id_);
    }

    uint Stage::GetId() const { return id_; }
} // namespace soil::video::shader
