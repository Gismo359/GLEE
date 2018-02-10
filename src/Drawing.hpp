//
// Created by BraynStorm on 2/10/18.
//

#ifndef GLEE_DRAWING_HPP
#define GLEE_DRAWING_HPP


#include <glm/glm.hpp>
#include <GL/gl.h>

#include <vector>

namespace glee {
    using glm::vec2;
    using glm::vec4;

    class Drawable {
      public:
        virtual void bindToVAO() const = 0;
        virtual ~Drawable() = 0;
    };

    class Line : public Drawable {
        void updateVertices() const;
        void updateColors() const;

      public:
        explicit Line(vec2&& end, bool isDynamic = false);
        explicit Line(vec2&& start, vec2&& end, bool isDynamic = false);

        Line(Line&&) = default;
        Line(const Line&);

        Line& operator=(Line&&) = default;
        Line& operator=(const Line&);

        virtual ~Line() override;

        void setStart(vec2&& start);
        void setEnd(vec2&& end);
        void setEndPoints(vec2&& start, vec2&& end);

        void setColors(uint32_t startARGB, uint32_t endARGB);
        void setStartColor(uint32_t startARGB);
        void setEndColor(uint32_t endARGB);

        vec2 getStart();
        vec2 getEnd();

        virtual void bindToVAO() const override;
        void draw();

      private:
        GLuint _vbo = 0;

        vec2 _startVertex;
        vec2 _endVertex;
        vec4 _startColor;
        vec4 _endColor;
    };

    class LineBatch {
      public:
        LineBatch();
        LineBatch(std::initializer_list<Line> lines);

        LineBatch(LineBatch&&) = default;
        LineBatch(const LineBatch&) = delete;

        LineBatch& operator=(LineBatch&&) = delete;
        LineBatch& operator=(const LineBatch&) = default;

        void addLine(Line&&);
        void addLine(std::vector<Line>::iterator, Line&&);
        void addLine(std::vector<Line>::const_iterator, Line&&);

        void draw();

      private:
        void cleanUp();
        void markDirty();
        bool isDirty() const;

        GLuint _vao;
        std::vector<Line> _lines;
        bool _dirty = true;
    };
};

#endif //GLEE_DRAWING_HPP
