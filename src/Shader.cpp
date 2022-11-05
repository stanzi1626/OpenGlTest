#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RenderID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RenderID = Load( source.VertexSource.c_str(), GL_VERTEX_SHADER, source.FragmentSource.c_str(), GL_FRAGMENT_SHADER, NULL );
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RenderID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath){
    std::ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)){
        if (line.find("#shader") != std::string::npos){
            if (line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}


GLuint Shader::Load( const char* shader, ... )
    {
        GLuint prog = glCreateProgram();
        va_list args;
        va_start( args, shader );
        while( shader )
        {
            const GLenum type = va_arg( args, GLenum );
            Shader::AttachShader( prog, type, shader );
            shader = va_arg( args, const char* );
        }
        va_end( args );
        glLinkProgram( prog );
        Shader::CheckStatus( prog );
        return prog;
    }
void Shader::CheckStatus( GLuint obj )
    {
        GLint status = GL_FALSE;
        if( glIsShader(obj) ) glGetShaderiv( obj, GL_COMPILE_STATUS, &status );
        if( glIsProgram(obj) ) glGetProgramiv( obj, GL_LINK_STATUS, &status );
        if( status == GL_TRUE ) return;
        GLchar log[ 1 << 15 ] = { 0 };
        if( glIsShader(obj) ) glGetShaderInfoLog( obj, sizeof(log), NULL, log );
        if( glIsProgram(obj) ) glGetProgramInfoLog( obj, sizeof(log), NULL, log );
        std::cerr << log << std::endl;
        std::exit( EXIT_FAILURE );
    }

void Shader::AttachShader( GLuint program, GLenum type, const char* src )
    {
        GLuint shader = glCreateShader( type );
        glShaderSource( shader, 1, &src, NULL );
        glCompileShader( shader );
        Shader::CheckStatus( shader );
        glAttachShader( program, shader );
        glDeleteShader( shader );
    }

void Shader::Bind() const{

    GLCall(glUseProgram(m_RenderID)); 

}

void Shader::Unbind() const {

    GLCall(glUseProgram(0));

}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RenderID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}