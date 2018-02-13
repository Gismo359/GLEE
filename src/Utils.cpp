//
// Created by braynstorm on 2/12/18.
//

#include "Utils.hpp"

namespace glee {

    vec4 argbToVec4(uint32_t argb) {
        const float a = ((argb & 0xFF000000) >> 24) / 255.f;
        const float r = ((argb & 0x00FF0000) >> 16) / 255.f;
        const float g = ((argb & 0xFF00) >> 8) / 255.f;
        const float b = (argb & 0xFF) / 255.f;

        auto result = vec4{ a, r, g, b };
        return result;
    }

    void deleteIfVAO(GLuint vao) {
        if (glIsVertexArray(vao) == GL_TRUE)
            glDeleteVertexArrays(1, &vao);
    }

}

