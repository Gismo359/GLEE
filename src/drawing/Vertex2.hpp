//
// Created by braynstorm on 2/13/18.
//

#ifndef GLEE_VERTEX_HPP
#define GLEE_VERTEX_HPP

#include <type_traits>
#include <glm/vec2.hpp>
#include <utility>
#include <glm/vec4.hpp>
#include "../Utils.hpp"

namespace glee {
    using glm::vec2;
    using glm::vec4;

    struct Vertex2 {
        vec2 position;
        vec2 texCoords;
        vec4 color;
      public:
        template<
            typename PosType,
            typename TexType,
            typename ColorType,
            typename = std::enable_if_t<std::is_assignable<vec2, PosType>::value>,
            typename = std::enable_if_t<std::is_assignable<vec2, TexType>::value>,
            typename = std::enable_if_t<std::is_assignable<vec4, ColorType>::value>
        >
        explicit Vertex2(
            PosType&& position,
            TexType&& textureCoords = TexType{ 0.f, 0.f },
            ColorType&& color = argbToVec4(0xFFFFFFFF)
        ) noexcept : position{ std::forward<PosType>(position) },
                     texCoords{ std::forward<TexType>(textureCoords) },
                     color{ std::forward<ColorType>(color) } {}

        Vertex2& setX(float x);
        Vertex2& setY(float y);
        Vertex2& setColor(uint32_t color);
        Vertex2& setTexCoordX(float x);
        Vertex2& setTexCoordY(float y);

        template<
            typename Vec2,
            typename = std::enable_if_t<std::is_assignable<vec2, Vec2>::value>
        >
        Vertex2& setPosition(Vec2&& position) {
            position = std::forward<Vec2>(position);
            return *this;
        }

        template<
            typename Vec2,
            typename = std::enable_if_t<std::is_assignable<vec2, Vec2>::value>
        >
        Vertex2& setTexCoord(Vec2&& texCoord) {
            texCoord = std::forward<Vec2>(texCoord);
            return *this;
        }

        template<
            typename Vec4,
            typename = std::enable_if_t<std::is_assignable<vec4, Vec4>::value>
        >
        Vertex2& setColor(Vec4&& color) {
            color = std::forward<Vec4>(color);
            return *this;
        }

    };
}
#endif //GLEE_VERTEX_HPP
