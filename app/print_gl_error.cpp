
#include "print_gl_error.hpp"


// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
bool printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        char* errorString = NULL;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString);
        return true;
    }
    return false;
}

void throwOnGLError() {
    if(printGLError()) {
        throw std::runtime_error("GL error!");
    }
}


