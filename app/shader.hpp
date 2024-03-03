#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/gl.h>
#include <string>

class Shader {
public:
    Shader(const char* name);

    void useProgram();

    GLint getUniformLocation(const char* name);
    
    GLint getAttributeLocation(const char* name);

private:

    void report_error(const std::string& file, GLuint obj);

    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;
};


#endif /* SHADER_HPP */
