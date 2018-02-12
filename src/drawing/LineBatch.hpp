//
// Created by BraynStorm on 2/10/18.
//

#ifndef GLEE_LINE_BATCH_HPP
#define GLEE_LINE_BATCH_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <string>
#include <type_traits>
#include <exception>

#include "Line.hpp"

namespace glee {
    using glm::vec2;
    using glm::vec4;

    class LineBatch {
      public:
        LineBatch() noexcept;
        LineBatch(std::initializer_list<Line> lines) noexcept;

        LineBatch(LineBatch&&) = default;
        LineBatch(const LineBatch&) = delete;

        LineBatch& operator=(LineBatch&&) = default;
        LineBatch& operator=(const LineBatch&) = delete;

        template<typename T>
        void addLine(T&& line) noexcept {
            _lines.emplace_back(std::forward<T>(line));
            markDirty();
        }

        void draw() noexcept;

        ~LineBatch();

      private:
        void cleanUp() noexcept;
        void markDirty() noexcept;
        bool isDirty() const noexcept;

        GLuint _vao;
        std::vector<Line> _lines;
        bool _dirty = true;
    };

};

#endif //GLEE_LINE_BATCH_HPP
