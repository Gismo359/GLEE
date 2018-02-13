//
// Created by BraynStorm on 2/10/18.
//

#include "../Utils.hpp"
#include "LineBatch.hpp"

namespace glee {
    using glm::vec2;

    GLuint createVAOLine(GLuint vbo) {
        GLuint vao;
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 4,
            (GLvoid*) 0
        );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * (4 + 2),
            (GLvoid*) (sizeof(float) * 2)
        );

        glBindVertexArray(0);

        return vao;
    }

    LineBatch::LineBatch() noexcept : _vao{ 0 } {}

    LineBatch::LineBatch(std::initializer_list<Line> lines) noexcept : _vao{ 0 } {
        _lines = lines;
        markDirty();
    }


    void LineBatch::draw(bool useVAO) noexcept {
        if (useVAO) {
            if (isDirty())
                cleanUp();

            glBindVertexArray(_vao);
            glDrawArrays(GL_LINES, 0, _lines.size() * 2);
            glBindVertexArray(0);
        } else {
            for (const auto& line : _lines) {
                line.draw();
            }
        }
    }

    void LineBatch::cleanUp() noexcept {
        _dirty = false;
    }

    void LineBatch::markDirty() noexcept {
        _dirty = true;
    }

    bool LineBatch::isDirty() const noexcept {
        return _dirty;
    }

    LineBatch::~LineBatch() {
        deleteIfVAO(_vao);
    }

};