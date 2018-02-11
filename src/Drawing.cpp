//
// Created by BraynStorm on 2/10/18.
//

#include "Drawing.hpp"

namespace glee {
    using glm::vec2;

    Drawable::~Drawable() = default;

    Line::Line(vec2&& end, bool dynamic) : Line(vec2{ 0.0, 0.0 },
                                                std::move(end), dynamic) {}

    Line::Line(vec2&& start, vec2&& end, bool dynamic) : _startVertex(std::move(start)),
                                                         _endVertex(std::move(end)) {

        // generate the VBO for the line drawing
        glGenBuffers(1, &_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            (2 * (2 + 4)) * sizeof(float),
            nullptr,
            dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
        );
        updateVertices();
        updateColors();
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void Line::updateVertices() const {
        const float data[]{ _startVertex.x, _startVertex.y, _endVertex.x, _endVertex.y };

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void Line::updateColors() const {
        const float data[]{
            _startColor.x, _startColor.y, _startColor.z, _startColor.w,
            _endColor.x, _endColor.y, _endColor.z, _endColor.w,
        };

//        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//        glBufferSubData(GL_ARRAY_BUFFER, 4 * sizeof(float), 4 * 2 * sizeof(float), data);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Line::bindToVAO() const {
//        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//
//        glVertexAttribPointer(
//            0,
//            2,
//            GL_FLOAT,
//            GL_FALSE,
//            0,
//            nullptr
//        );
//
//        glVertexAttribPointer(
//            1,
//            4,
//            GL_FLOAT,
//            GL_FALSE,
//            4 * sizeof(float),
//            nullptr
//        );
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    vec2 Line::getStart() { return _startVertex; }

    vec2 Line::getEnd() { return _endVertex; }

    Line& Line::setStart(vec2&& start) {
        _startVertex = start;
        updateVertices();
        return *this;
    }

    Line& Line::setEnd(vec2&& end) {
        _endVertex = end;
        updateVertices();
        return *this;
    }

    Line& Line::setEndPoints(vec2&& start, vec2&& end) {
        _startVertex = start;
        _endVertex = end;
        updateVertices();
        return *this;
    }

    void Line::draw() {
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

        glDrawArrays(GL_LINES, 0, 1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Line::Line(const Line& copy) {
        setEndPoints(vec2{ copy._startVertex }, vec2{ copy._endVertex });
    }

    Line& Line::operator=(const Line& rhs) {
        setEndPoints(vec2{ rhs._startVertex }, vec2{ rhs._endVertex });
        return *this;
    }

    Line::~Line() {
        glDeleteBuffers(1, &_vbo);
    }

    vec4 fromUINT32(uint32_t argb) {
        const float a = ((argb & 0xFF000000) >> 24) / 255.f;
        const float r = ((argb & 0x00FF0000) >> 16) / 255.f;
        const float g = ((argb & 0xFF00) >> 8) / 255.f;
        const float b = (argb & 0xFF) / 255.f;

        auto result = vec4{ a, r, g, b };
        return result;
    }


    Line& Line::setColors(uint32_t startARGB, uint32_t endARGB) {
        _startColor = fromUINT32(startARGB);
        _endColor = fromUINT32(endARGB);

        updateColors();
        return *this;
    }

    Line& Line::setStartColor(uint32_t startARGB) {
        _startColor = fromUINT32(startARGB);
        updateColors();
        return *this;
    }

    Line& Line::setEndColor(uint32_t endARGB) {
        _endColor = fromUINT32(endARGB);
        updateColors();
        return *this;
    }

    LineBatch::LineBatch() : _vao{ 0 } {
        glGenVertexArrays(1, &_vao);

        glBindVertexArray(_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    LineBatch::LineBatch(std::initializer_list<Line> lines) : LineBatch{ } {
        _lines = lines;
        markDirty();
    }

    void LineBatch::addLine(Line&& line) {
        _lines.emplace_back(line);
        markDirty();
    }

    void LineBatch::addLine(Line line) {
        _lines.emplace_back(std::move(line));
        markDirty();
    }

    void LineBatch::addLine(std::vector<Line>::iterator iterator, Line&& line) {
        _lines.emplace(iterator, line);
        markDirty();
    }

    void LineBatch::addLine(std::vector<Line>::const_iterator iterator,
                            Line&& line) {
        _lines.emplace(iterator, line);
        markDirty();
    }

    void LineBatch::draw() {
        if (isDirty())
            cleanUp();

        glBindVertexArray(_vao);
        // TODO use gsl::narrow_cast instead.
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(_lines.size()));
        glBindVertexArray(0);
    }

    void LineBatch::cleanUp() {
        glBindVertexArray(_vao);

        for (const Line& line :_lines) {
            line.bindToVAO();
        }

        glBindVertexArray(0);
        _dirty = false;
    }

    void LineBatch::markDirty() {
        _dirty = true;
    }

    bool LineBatch::isDirty() const {
        return _dirty;
    }

};