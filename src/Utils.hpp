//
// Created by braynstorm on 2/12/18.
//

#ifndef GLEE_UTILS_HPP
#define GLEE_UTILS_HPP

#include <GL/glew.h>

namespace glee{
    void deleteIfVAO(GLuint vao) {
        if (glIsVertexArray(vao) == GL_TRUE)
            glDeleteVertexArrays(1, &vao);
    }
};
#endif //GLEE_UTILS_HPP
