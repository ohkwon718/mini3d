#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Platform/GlfwRuntime.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ShaderProgram.hpp"

int main()
{
    GlfwRuntime glfw;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWWindowPtr window{
        glfwCreateWindow(800, 600, "mini3d", nullptr, nullptr)
    };

    if (!window) {
        return 1;
    }
    glfwMakeContextCurrent(window.get());

    if (!gladLoadGL(glfwGetProcAddress)) {
        return 1;
    }    

    VertexArray vao;
    vao.bind();
    VertexBuffer vbo;

    std::vector<float> vertices{
        0.0f, 0.5f, 0.0f,
        -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f
    };
    vbo.upload(
        vertices.data(),
        vertices.size() * sizeof(float)
    );
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );
    glEnableVertexAttribArray(0);

    VertexArray::unbind();

    const char* vertexSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPosition;

    void main()
    {
        gl_Position = vec4(aPosition, 1.0);
    }
    )";

    const char* fragmentSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
    )";

    ShaderProgram shader(vertexSource, fragmentSource);

    while (!glfwWindowShouldClose(window.get())) {
        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // glfwSwapBuffers(window.get());
        // glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }


    return 0;
}