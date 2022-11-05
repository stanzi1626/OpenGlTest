#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "SDL.h"
#include "glm.hpp"

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RenderID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set uniforms:
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v3, float v4);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    GLuint Load( const char* shader, ... );
    void CheckStatus(GLuint obj);
    void AttachShader(GLuint program, GLenum type, const char* src);
    int GetUniformLocation(const std::string& name);
};