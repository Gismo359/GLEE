//
// Created by braynstorm on 2/13/18.
//
#include "Vertex2.hpp"
#include <cstring>
#include <memory>
#include <vector>

namespace glee {
    Vertex2& Vertex2::setX(float x) {
        position.x = x;
        return *this;
    }

    Vertex2& Vertex2::setY(float y) {
        position.y = y;
        return *this;
    }

    Vertex2& Vertex2::setColor(uint32_t color) {
        Vertex2::color = argbToVec4(color);
        return *this;
    }

    Vertex2& Vertex2::setTexCoordX(float x) {
        texCoords.s = x;
        return *this;
    }

    Vertex2& Vertex2::setTexCoordY(float y) {
        texCoords.t = y;
        return *this;
    }

}