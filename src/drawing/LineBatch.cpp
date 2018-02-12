//
// Created by BraynStorm on 2/10/18.
//

#include "../Utils.hpp"
#include "LineBatch.hpp"

namespace glee {
    using glm::vec2;


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
            for(const auto& line : _lines){
                line.draw();
            }
        }
    }

    void LineBatch::cleanUp() noexcept {
        deleteIfVAO(_vao);
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        for (const auto& line : _lines) {
            line.bindToVAO();
        }
        glBindVertexArray(0);
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