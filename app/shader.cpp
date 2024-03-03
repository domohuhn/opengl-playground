#include "shader.hpp"
#include "print_gl_error.hpp"

#include <string>
#include <fstream>
#include <streambuf>

std::string loadFile(const std::string& name) {

    std::ifstream t(name);
    if(!t.good()) {
        throw std::runtime_error(name + " not found!");
    }
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

Shader::Shader(const char* name)
{
    const auto vertexSourcePath = std::string("materials/") + name + "_vert.glsl";
    const auto fragmentSourcePath = std::string("materials/") + name + "_frag.glsl";
    const auto vertexSource = loadFile(vertexSourcePath);
    const auto fragmentSource = loadFile(fragmentSourcePath);

    const char* vertexSourcePtr = vertexSource.c_str();
    const char* fragmentSourcePtr = fragmentSource.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexSourcePtr, NULL);
    throwOnGLError();
    glCompileShader(vertex_shader);
    report_error(vertexSourcePath,vertex_shader);
    throwOnGLError();
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentSourcePtr, NULL);
    throwOnGLError();
    glCompileShader(fragment_shader);
    report_error(fragmentSourcePath,fragment_shader);
    throwOnGLError();
 
    program = glCreateProgram();
    throwOnGLError();
    glAttachShader(program, vertex_shader);
    throwOnGLError();
    glAttachShader(program, fragment_shader);
    throwOnGLError();
    glLinkProgram(program);
    throwOnGLError();

}


GLint Shader::getUniformLocation(const char* name) {
    GLint v =  glGetUniformLocation(program, name);
    throwOnGLError();
    return v;
}

GLint Shader::getAttributeLocation(const char* name) {
    GLint v =  glGetAttribLocation(program, name);
    throwOnGLError();
    return v;
}


void Shader::useProgram()
{
    glUseProgram(program);
    throwOnGLError();
}

void Shader::report_error(const std::string& file, GLuint obj)
{
    GLint shaderCompiled;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shaderCompiled);

    if(shaderCompiled == GL_FALSE)
    {
        int infologLength = 0;

        int charsWritten  = 0;
        char *infoLog;

        glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

        if (infologLength > 0)
        {
            infoLog = (char *)malloc(infologLength);
            glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
            printf("Error wile compiling '%s':\n%s\n\nend of error\n", file.c_str(), infoLog);
            free(infoLog);
        }
    }
}
