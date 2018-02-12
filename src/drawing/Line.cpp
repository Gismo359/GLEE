//
// Created by braynstorm on 2/12/18.
//

#include "Line.hpp"

namespace glee {
    using glm::vec2;
    using glm::vec4;

    GLuint createVBOLine(bool dynamic) {
        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            (2 + 4) * 2 * sizeof(float),
            nullptr,
            dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return vbo;
    }

    void Line::updateVertices() const {
        const float data[]{ _startVertex.x, _startVertex.y, _endVertex.x, _endVertex.y };

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Line::updateColors() const {
        const float data[]{
            _startColor.w, _startColor.x, _startColor.y, _startColor.z,
            _endColor.w, _endColor.x, _endColor.y, _endColor.z,
        };

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 4 * sizeof(float), 4 * 2 * sizeof(float), data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Line::bindToVAO() const {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) 0
        );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) (sizeof(float) * 2 * 2)
        );

    }

    void Line::draw() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) 0
        );

        glVertexAttribPointer(
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) (sizeof(float) * 4)
        );

        glDrawArrays(GL_LINES, 0, 2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Line::Line(const Line& copy) noexcept: _vbo{ createVBOLine(false) } {
        setEndPoints(vec2{ copy._startVertex }, vec2{ copy._endVertex });
        setColors(vec4{ copy._startColor }, vec4{ copy._endColor });
    }

    Line& Line::operator=(const Line& copy) noexcept {
        _vbo = createVBOLine(false);
        setEndPoints(vec2{ copy._startVertex }, vec2{ copy._endVertex });
        setColors(vec4{ copy._startColor }, vec4{ copy._endColor });
        return *this;
    }

    Line::~Line() {
        if (glIsBuffer(_vbo) == GL_TRUE)
            glDeleteBuffers(1, &_vbo);
    }

    vec4 argbToVec4(uint32_t argb) {
        const float a = ((argb & 0xFF000000) >> 24) / 255.f;
        const float r = ((argb & 0x00FF0000) >> 16) / 255.f;
        const float g = ((argb & 0xFF00) >> 8) / 255.f;
        const float b = (argb & 0xFF) / 255.f;

        auto result = vec4{ a, r, g, b };
        return result;
    }

    Line::Line(Line&& line)
    noexcept : _vbo{ line._vbo },
               _startVertex{ std::forward<vec2&&>(line._startVertex) },
               _endVertex{ std::forward<vec2&&>(line._endVertex) },
               _startColor{ std::forward<vec4&&>(line._startColor) },
               _endColor{ std::forward<vec4&&>(line._endColor) } {
        line._vbo = 0;
    }

    Line& Line::operator=(Line&& line) noexcept {
        _vbo = line._vbo;
        line._vbo = 0;
        _startVertex = std::forward<vec2&&>(line._startVertex);
        _endVertex = std::forward<vec2&&>(line._endVertex);
        _startColor = std::forward<vec4&&>(line._startColor);
        _endColor = std::forward<vec4&&>(line._endColor);
        return *this;
    }


}

