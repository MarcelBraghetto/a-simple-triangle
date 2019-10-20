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
    const GLuint attributeLocationTexCoord;
    const GLsizei stride;
    const GLsizei offsetPosition;
    const GLsizei offsetTexCoord;

    Internal(const std::string& shaderName)
        : shaderProgramId(::createShaderProgram(shaderName)),
          uniformLocationMVP(glGetUniformLocation(shaderProgramId, "u_mvp")),
          attributeLocationVertexPosition(glGetAttribLocation(shaderProgramId, "a_vertexPosition")),
          attributeLocationTexCoord(glGetAttribLocation(shaderProgramId, "a_texCoord")),
          stride(5 * sizeof(float)),
          offsetPosition(0),
          offsetTexCoord(3 * sizeof(float)) {}

    void render(const ast::OpenGLMesh& mesh, const ast::OpenGLTexture& texture, const glm::mat4& mvp) const
    {
        // Instruct OpenGL to starting using our shader program.
        glUseProgram(shaderProgramId);

        // Populate the 'u_mvp' uniform in the shader program.
        glUniformMatrix4fv(uniformLocationMVP, 1, GL_FALSE, &mvp[0][0]);

        // Activate the 'a_vertexPosition' attribute and specify how it should be configured.
        glEnableVertexAttribArray(attributeLocationVertexPosition);
        glVertexAttribPointer(attributeLocationVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetPosition));

        // Activate the 'a_texCoord' attribute and specify how it should be configured.
        glEnableVertexAttribArray(attributeLocationTexCoord);
        glVertexAttribPointer(attributeLocationTexCoord, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetTexCoord));

        // Apply the texture we want to paint the mesh with.
        texture.bind();

        // Bind the vertex and index buffers.
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBufferId());

        // Execute the draw command - with how many indices to iterate.
        glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, (GLvoid*)0);

        // Tidy up.
        glDisableVertexAttribArray(attributeLocationVertexPosition);
        glDisableVertexAttribArray(attributeLocationTexCoord);
    }

    ~Internal()
    {
        glDeleteProgram(shaderProgramId);
    }
};

OpenGLPipeline::OpenGLPipeline(const std::string& shaderName)
    : internal(ast::make_internal_ptr<Internal>(shaderName)) {}

void OpenGLPipeline::render(const ast::OpenGLMesh& mesh, const ast::OpenGLTexture& texture, const glm::mat4& mvp) const
{
    internal->render(mesh, texture, mvp);
}
