#ifndef PRINT_GL_ERROR_H
#define PRINT_GL_ERROR_H

#include <glad/gl.h>
#include <stdio.h>
#include <stdexcept>


// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
bool printGLError();

void throwOnGLError();



#endif /* PRINT_GL_ERROR_H */
