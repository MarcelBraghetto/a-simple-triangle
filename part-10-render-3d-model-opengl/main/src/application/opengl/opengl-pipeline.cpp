#include "opengl-pipeline.hpp"
#include "../../core/assets.hpp"
#include "../../core/log.hpp"
#include <stdexcept>
#include <vector>

using ast::OpenGLPipeline;

namespace
{
    GLuint compileShader(const GLenum& shaderType, const std::string& shaderSource)
    {
        const std::string logTag{"ast::OpenGLPipeline::compileShader"};
        GLuint shaderId{glCreateShader(shaderType)};

        const char* shaderData{shaderSource.c_str()};
        glShaderSource(shaderId, 1, &shaderData, nullptr);
        glCompileShader(shaderId);

        GLint shaderCompilationResult;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompilationResult);

        if (!shaderCompilationResult)
        {
            GLint errorMessageLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
            std::vector<char> errorMessage(errorMessageLength + 1);
            glGetShaderInfoLog(shaderId, errorMessageLength, nullptr, &errorMessage[0]);
            ast::log(logTag, &errorMessage[0]);
            throw std::runtime_error(logTag + "Shader failed to compile.");
        }

        return shaderId;
    }

    GLuint createShaderProgram(const std::string& shaderName)
    {
        const std::string logTag{"ast::OpenGLPipeline::createShaderProgram"};

        ast::log(logTag, "Creating pipeline for '" + shaderName + "'");

        const std::string vertexShaderCode{ast::assets::loadTextFile("assets/shaders/opengl/" + shaderName + ".vert")};
        const std::string fragmentShaderCode{ast::assets::loadTextFile("assets/shaders/opengl/" + shaderName + ".frag")};

#ifdef USING_GLES
        std::string vertexShaderSource{"#version 100\n" + vertexShaderCode};
        std::string fragmentShaderSource{"#version 100\nprecision mediump float;\n" + fragmentShaderCode};
#else
        std::string vertexShaderSource{"#version 120\n" + vertexShaderCode};
        std::string fragmentShaderSource{"#version 120\n" + fragmentShaderCode};
#endif

        GLuint shaderProgramId{glCreateProgram()};
        GLuint vertexShaderId{::compileShader(GL_VERTEX_SHADER, vertexShaderSource)};
        GLuint fragmentShaderId{::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource)};

        glAttachShader(shaderProgramId, vertexShaderId);
        glAttachShader(shaderProgramId, fragmentShaderId);
        glLinkProgram(shaderProgramId);

        GLint shaderProgramLinkResult;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &shaderProgramLinkResult);

        if (!shaderProgramLinkResult)
        {
            GLint errorMessageLength;
            glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &errorMessageLength);
            std::vector<char> errorMessage(errorMessageLength + 1);
            glGetProgramInfoLog(shaderProgramId, errorMessageLength, nullptr, &errorMessage[0]);
            ast::log(logTag, &errorMessage[0]);
            throw std::runtime_error(logTag + "Shader program failed to compile.");
        }

        glDetachShader(shaderProgramId, vertexShaderId);
        glDetachShader(shaderProgramId, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return shaderProgramId;
    }
} // namespace

struct OpenGLPipeline::Internal
{
    const GLuint shaderProgramId;
    const GLuint uniformLocationMVP;
    const GLuint attributeLocationVertexPosition;

    Internal(const std::string& shaderName)
        : shaderProgramId(::createShaderProgram(shaderName)),
          uniformLocationMVP(glGetUniformLocation(shaderProgramId, "mvp")),
          attributeLocationVertexPosition(glGetAttribLocation(shaderProgramId, "vertexPosition")) {}

    void render(const ast::OpenGLMesh& mesh, const glm::mat4& mvp) const
    {
#ifndef USING_GLES
        // Render in wire frame for now until we put lighting and texturing in.
        // Note that this is not supported on OpenGL ES
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

        // Instruct OpenGL to starting using our shader program.
        glUseProgram(shaderProgramId);

        // Populate the 'mvp' uniform in the shader program.
        glUniformMatrix4fv(uniformLocationMVP, 1, GL_FALSE, &mvp[0][0]);

        // Activate the 'vertexPosition' attribute and specify how it should be configured.
        glEnableVertexAttribArray(attributeLocationVertexPosition);
        glVertexAttribPointer(attributeLocationVertexPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Bind the vertex and index buffers.
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBufferId());

        // Execute the draw command - with how many indices to iterate.
        glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, (void*)0);

        // Tidy up.
        glDisableVertexAttribArray(attributeLocationVertexPosition);
    }

    ~Internal()
    {
        glDeleteProgram(shaderProgramId);
    }
};

OpenGLPipeline::OpenGLPipeline(const std::string& shaderName)
    : internal(ast::make_internal_ptr<Internal>(shaderName)) {}

void OpenGLPipeline::render(const ast::OpenGLMesh& mesh, const glm::mat4& mvp) const
{
    internal->render(mesh, mvp);
}
