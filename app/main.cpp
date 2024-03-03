#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "print_gl_error.hpp"
#include "shader.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
 
#include <stdlib.h>
#include <stdio.h>
 
static const struct
{
    float x, y;
    float r, g, b;
} vertices[] =
{
    { -1.0f, -1.0f, 1.f, 0.f, 0.f },
    {  1.0f, -1.0f, 0.f, 1.f, 0.f },
    {  -1.0f,  1.0f, 0.f, 0.f, 1.f },
    {  1.0f,  1.0f, 0.f, 1.f, 1.f },
};

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

 
int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer;
 
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(640, 640, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    
    glfwSwapInterval(1);
 
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader shader("quad");
    
    const GLint vpos_location = shader.getAttributeLocation("vPos");
    const GLint vcol_location = shader.getAttributeLocation("vCol");
    const GLint uMeta_location = shader.getUniformLocation("uMeta");
    const GLint uBufferSize_location = shader.getUniformLocation("uBufferSize");
    const GLint uHeight_location = shader.getUniformLocation("uHeight");

    const GLint upperFadeLimitLeft_location  = shader.getUniformLocation("upperFadeLimitLeft");
    const GLint upperFadeWidthLeft_location  = shader.getUniformLocation("upperFadeWidthLeft");
    const GLint lowerFadeWidthLeft_location  = shader.getUniformLocation("lowerFadeWidthLeft");

    const GLint upperFadeLimitRight_location = shader.getUniformLocation("upperFadeLimitRight");
    const GLint upperFadeWidthRight_location = shader.getUniformLocation("upperFadeWidthRight");
    const GLint lowerFadeWidthRight_location = shader.getUniformLocation("lowerFadeWidthRight");

    float upperFadeLimitLeft_value  = 600.0;
    float upperFadeWidthLeft_value  = 60.0;
    float lowerFadeWidthLeft_value  = 180.0;
    float upperFadeLimitRight_value = 550.0;
    float upperFadeWidthRight_value = 180.0;
    float lowerFadeWidthRight_value = 60.0;

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));
    
    int counter = 0;
    constexpr std::size_t dataSize = 64;
    GLuint data[dataSize]{};
    GLuint uDataHeight = 32;
    GLuint bufferSize[2]{};

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    while (!glfwWindowShouldClose(window))
    {
        
        int width, height;

        for(size_t i=0;i<dataSize;++i) {
            data[i] = (counter+i)%255;
        }
        data[dataSize-1] = 0;
        counter = (counter+1)%255;
 
        glfwGetFramebufferSize(window, &width, &height);
        bufferSize[0] = width;
        bufferSize[1] = height;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

// =============================================
bool my_tool_active = true;
ImGui::Begin("Uniform editor", &my_tool_active, ImGuiWindowFlags_MenuBar);

ImGui::SliderFloat("upperFadeLimitLeft", &upperFadeLimitLeft_value , 0.0, height);
ImGui::SliderFloat("upperFadeWidthLeft", &upperFadeWidthLeft_value , 0.0, width/2.0);
ImGui::SliderFloat("lowerFadeWidthLeft", &lowerFadeWidthLeft_value , 0.0, width/2.0);
ImGui::SliderFloat("upperFadeLimitRight", &upperFadeLimitRight_value, 0.0, height);
ImGui::SliderFloat("upperFadeWidthRight", &upperFadeWidthRight_value, 0.0, width/2.0);
ImGui::SliderFloat("lowerFadeWidthRight", &lowerFadeWidthRight_value, 0.0, width/2.0);
int dataHeight = uDataHeight;
ImGui::SliderInt("uDataHeight", &dataHeight, 0, 64);
uDataHeight = dataHeight;

ImGui::End();

// =============================================

 
        glViewport(0, 0, width, height);
        printGLError();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        printGLError();
        glClear(GL_COLOR_BUFFER_BIT);
        printGLError();
 
        shader.useProgram();
        printGLError();
        glUniform1uiv(uMeta_location, dataSize, data);
        printGLError();
        
        glUniform1uiv(uBufferSize_location, 2, bufferSize);
        printGLError();

        glUniform1ui(uHeight_location, uDataHeight);
        printGLError();

        glUniform1f(upperFadeLimitLeft_location , upperFadeLimitLeft_value );
        glUniform1f(upperFadeWidthLeft_location , upperFadeWidthLeft_value );
        glUniform1f(lowerFadeWidthLeft_location , lowerFadeWidthLeft_value );
        glUniform1f(upperFadeLimitRight_location, upperFadeLimitRight_value);
        glUniform1f(upperFadeWidthRight_location, upperFadeWidthRight_value);
        glUniform1f(lowerFadeWidthRight_location, lowerFadeWidthRight_value);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        printGLError();
 
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
 