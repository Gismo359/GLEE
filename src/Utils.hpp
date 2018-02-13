//
// Created by braynstorm on 2/12/18.
//

#ifndef GLEE_UTILS_HPP
#define GLEE_UTILS_HPP

#include <GL/glew.h>
#include <glm/vec4.hpp>

namespace glee{
    using glm::vec4;

    void deleteIfVAO(GLuint vao) ;
    vec4 argbToVec4(uint32_t argb);

};
#endif //GLEE_UTILS_HPP
