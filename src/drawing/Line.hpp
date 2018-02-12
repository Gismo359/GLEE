//
// Created by braynstorm on 2/12/18.
//

#ifndef GLEE_LINE_HPP
#define GLEE_LINE_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <string>
#include <type_traits>
#include <exception>

namespace glee {
    using glm::vec2;
    using glm::vec4;

    vec4 argbToVec4(uint32_t argb);
    GLuint createVBOLine(bool);

    class Line {
        void updateVertices() const;
        void updateColors() const;

      public:
        explicit Line(Line&&) noexcept;
        Line(const Line&) noexcept;

        Line& operator=(Line&&) noexcept;
        Line& operator=(const Line&) noexcept;

        template<typename Vec2_t, typename = std::enable_if_t<std::is_assignable<Vec2_t, vec2>::value>>
        explicit Line(Vec2_t&& start, Vec2_t&& end, bool dynamic = false) noexcept:
            _vbo{ createVBOLine(dynamic) },
            _startVertex{ std::forward<Vec2_t>(start) },
            _endVertex{ std::forward<Vec2_t>(end) } {
            updateVertices();
            updateColors();
        }

        template<typename Vec2_t, typename = std::enable_if_t<std::is_assignable<Vec2_t, vec2>::value>>
        Line& setStartVertex(Vec2_t&& vertex) noexcept {
            _startVertex = std::forward<Vec2_t>(vertex);
            updateVertices();
            return *this;
        }

        template<typename Vec2_t, typename = std::enable_if_t<std::is_assignable<Vec2_t, vec2>::value>>
        Line& setEndVertex(Vec2_t&& vertex) noexcept {
            _endVertex = std::forward<Vec2_t>(vertex);
            updateVertices();
            return *this;
        }

        template<typename Vec2_t, typename = std::enable_if_t<std::is_assignable<Vec2_t, vec2>::value>>
        Line& setEndPoints(Vec2_t&& start, Vec2_t&& end) noexcept {
            _startVertex = std::forward<Vec2_t>(start);
            _endVertex = std::forward<Vec2_t>(end);
            updateVertices();
            return *this;
        };

        Line& setColors(uint32_t startARGB, uint32_t endARGB) noexcept {
            _startColor = argbToVec4(startARGB);
            _endColor = argbToVec4(endARGB);
            updateColors();
            return *this;
        }

        Line& setStartColor(uint32_t startARGB) noexcept {
            _startColor = argbToVec4(startARGB);
            updateColors();
            return *this;
        }

        Line& setEndColor(uint32_t endARGB) noexcept {
            _endColor = argbToVec4(endARGB);
            updateColors();
            return *this;
        }

        template<typename Vec4_t, typename = std::enable_if_t<std::is_assignable<vec4, Vec4_t>::value> >
        Line& setColors(Vec4_t&& startARGB, Vec4_t&& endARGB) noexcept {
            _startColor = std::forward<Vec4_t>(startARGB);
            _endColor = std::forward<Vec4_t>(endARGB);
            updateColors();
            return *this;
        }

        template<typename Vec4_t, typename = std::enable_if_t<std::is_assignable<Vec4_t, vec4>::value>>
        Line& setStartColor(Vec4_t&& startARGB) noexcept {
            _startColor = std::forward<Vec4_t>(startARGB);
            updateColors();
            return *this;
        }

        template<typename Vec4_t, typename = std::enable_if_t<std::is_assignable<Vec4_t, vec4>::value>>
        Line& setEndColor(Vec4_t&& endARGB) noexcept {
            _endColor = std::forward<Vec4_t>(endARGB);
            updateColors();
            return *this;
        }

        const vec2& getStartVertex() const noexcept { return _startVertex; }

        const vec2& getEndVertex() const noexcept { return _endVertex; }

        const vec4& getStartColor() const noexcept { return _startColor; }

        const vec4& getEndColor() const noexcept { return _endColor; }

        const vec2& operator[](int index) const {
            if (index == 0) {
                return _startVertex;
            }

            if (index == 1) {
                return _endVertex;
            }

            throw std::exception{ };
        }

        void draw();
        void bindToVAO() const;

        ~Line();

      private:
        GLuint _vbo;

        vec2 _startVertex;
        vec2 _endVertex;
        vec4 _startColor{ 1.0, 1.0, 1.0, 1.0 };
        vec4 _endColor{ 1.0, 1.0, 1.0, 1.0 };
    };
}
#endif //GLEE_LINE_HPP
