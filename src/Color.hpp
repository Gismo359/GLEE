//
// Created by braynstorm on 2/12/18.
//

#ifndef GLEE_COLOR_HPP
#define GLEE_COLOR_HPP

#include "Utils.hpp"
#include "glm/vec4.hpp"

namespace glee {
    using glm::vec4;
    namespace Color {
        const auto BLACK = argbToVec4(0x000000 | 0xFF000000);
        const auto WHITE = argbToVec4(0xFFFFFF | 0xFF000000);
        const auto RED = argbToVec4(0xFF0000 | 0xFF000000);
        const auto GREEN = argbToVec4(0x00FF00 | 0xFF000000);
        const auto BLUE = argbToVec4(0x0000FF | 0xFF000000);
        const auto CYAN = argbToVec4(0x00FFFF | 0xFF000000);
        const auto MAGENTA = argbToVec4(0xFF00FF | 0xFF000000);
        const auto YELLOW = argbToVec4(0xFFFF00 | 0xFF000000);


    };


}
#endif //GLEE_COLOR_HPP
