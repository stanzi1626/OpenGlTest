#pragma once
#include <signal.h>
#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if (!(x)) raise(SIGABRT);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT (GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:
    void Clear() const;
    void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};