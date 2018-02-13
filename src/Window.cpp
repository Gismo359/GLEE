//
// Created by braynstorm on 2/10/18.
//

#include <GL/glew.h>
#include <sstream>
#include <fstream>
#include "Window.hpp"
#include "Log.hpp"


#define LOG_GL_ERROR {\
    auto error = glGetError();\
    if (error) LOG(WARN, "GL_ERROR: %d -> %s", error, glewGetErrorString(error));\
};

namespace glee {
    using string= std::string;

    std::string readAllLines(std::string path){
        std::ifstream file(path);
        std::string line;
        std::ostringstream stream;
        while(std::getline(file, line)){
            stream << line << '\n';
        }

        return stream.str();
    }

    void logGlShaderErrors(GLuint shader) {
        GLint len = 100;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(static_cast<unsigned long>(len), ' ');
        glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), &len, &log[0]);

        if (log.size() > 0) {
            auto infoLogString = string(std::begin(log), std::end(log));

            LOG(ERROR, "Couldn't compile shaders. %s", infoLogString.c_str());
        }
    }

    void Window::compileShaderProgram() {
        _shaderProgram = glCreateProgram();
        auto shaderVertexSource = readAllLines("../shader_v.glsl");
        auto shaderFragmentSource = readAllLines("../shader_f.glsl");

        auto shaderVertex = glCreateShader(GL_VERTEX_SHADER);
        auto shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);

        auto src_vertex = shaderVertexSource.c_str();
        auto src_vertex_size = static_cast<GLint>(shaderVertexSource.size());

        auto src_fragment = shaderFragmentSource.c_str();
        auto src_fragment_size = static_cast<GLint>(shaderFragmentSource.size());

        glShaderSource(shaderVertex, 1, &src_vertex, &src_vertex_size);
        glShaderSource(shaderFragment, 1, &src_fragment, &src_fragment_size);

        glCompileShader(shaderVertex);
        LOG_GL_ERROR
        logGlShaderErrors(shaderVertex);
        glCompileShader(shaderFragment);
        LOG_GL_ERROR
        logGlShaderErrors(shaderFragment);

        glAttachShader(_shaderProgram, shaderVertex);
        LOG_GL_ERROR
        glAttachShader(_shaderProgram, shaderFragment);
        LOG_GL_ERROR

//        glBindFragDataLocation(_shaderProgram, 0, "pixelColor");
        LOG_GL_ERROR

        glLinkProgram(_shaderProgram);
        LOG_GL_ERROR

        GLint isLinked = 0;
        glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {

            GLint maxLength = 0;
            glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(static_cast<unsigned long>(maxLength));
            glGetProgramInfoLog(_shaderProgram, maxLength, &maxLength, &infoLog[0]);

            auto infoLogString = string(std::begin(infoLog), std::end(infoLog));
            LOG(ERROR, "Couldn't link shaders. %s", infoLogString.c_str());

            glDeleteProgram(_shaderProgram);
        }


        glValidateProgram(_shaderProgram);
        LOG_GL_ERROR

        GLint isValid = 0;
        glGetProgramiv(_shaderProgram, GL_VALIDATE_STATUS, &isValid);
        if (isValid == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(static_cast<unsigned long>(maxLength));
            glGetProgramInfoLog(_shaderProgram, maxLength, &maxLength, &infoLog[0]);

            auto infoLogString = string(std::begin(infoLog), std::end(infoLog));

            LOG(ERROR, "Couldn't link shaders. %s", infoLogString.c_str());

            glDeleteProgram(_shaderProgram);
        }

        glUseProgram(_shaderProgram);
        LOG_GL_ERROR

        _uniformProjectionMatrix = glGetUniformLocation(_shaderProgram, "projectionMatrix");
        _uniformModelViewMatrix = glGetUniformLocation(_shaderProgram, "modelViewMatrix");

        float matrixIdentity[]{
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        };

        glUniformMatrix4fv(_uniformProjectionMatrix, 1, GL_FALSE, matrixIdentity);
        LOG_GL_ERROR
        glUniformMatrix4fv(_uniformModelViewMatrix, 1, GL_FALSE, matrixIdentity);
        LOG_GL_ERROR
    }
    std::set<Window *> Window::_windowTracker;

    void Window::Render(uint32_t delta) {
        for (auto &&window : _windowTracker) { window->render(delta); }
    }

    void Window::callRenderCallbacks(Uint32 delta) {
        for (auto&& function : _renderCallbacks) { function(*this, delta); }
    }

    void Window::callEventCallbacks(Uint32 delta, SDL_Event event) {
        for (auto&& function : _eventCallbacks) {
            // TODO: Possibly add a handled flag via a wrapper and make EventCallback return void?
            if (function(*this, delta, event))
                // TODO: Do something else once handled?
                break;
        }

    Window::Window(const std::string &title, int x, int y, int width, int height) {

        _sdlWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
        if (!_sdlWindow) { LOG(ERROR, "Failed to initialize window: %s", SDL_GetError()); }

        _sdlGlContext = SDL_GL_CreateContext(_sdlWindow);
        if (!_sdlGlContext) { LOG(ERROR, "Failed to initialize OpenGL context: %s", SDL_GetError()); }
        glewExperimental = true;
        auto error = glewInit();

        if (error) { LOG(INFO, "%s", glewGetErrorString(error)); }

        // Swap buffers at creation
        swapBuffers();
        makeCurrentContext();

        _windowTracker.insert(this);
    }

    // TODO: Maybe add separate wrappers for width/height
    void Window::getSize(int &width, int &height) const { SDL_GetWindowSize(_sdlWindow, &width, &height); }

    void Window::setSize(int width, int height) { SDL_SetWindowSize(_sdlWindow, width, height); }

    void Window::swapBuffer() { SDL_GL_SwapWindow(this->_sdlWindow); }

    void Window::addRenderHandler(RenderHandler callback, CallbackData data) { this->_renderCallbacks.push_back(std::make_pair(callback, data)); }

    void Window::render(uint32_t delta) const {
        makeCurrentContext();
        for (auto &&pair : this->_renderCallbacks) { pair.first(delta, pair.second); }
        swapBuffers();
    }

    void Window::swapBuffers() const { SDL_GL_SwapWindow(this->_sdlWindow); }

    int Window::makeCurrentContext() const {
        auto error = SDL_GL_MakeCurrent(this->_sdlWindow, this->_sdlGlContext);
        if (error) { LOG(ERROR, "Could not set OpenGL context to window: %s", SDL_GetError()); }
        return error;
    }

    std::string Window::getTitle() const { return SDL_GetWindowTitle(_sdlWindow); }

    void Window::setTitle(const std::string &title) { SDL_SetWindowTitle(_sdlWindow, title.c_str()); }

    uint32_t Window::getWindowId() const {
        uint32_t windowId = SDL_GetWindowID(this->_sdlWindow);
        if (!windowId) { LOG(ERROR, "Enable to retrieve window ID: %s", SDL_GetError()); }
        return windowId;
    }

    bool Window::operator<(Window other) const { return this->_sdlWindow < other._sdlWindow; }

    bool Window::operator>(Window other) const { return this->_sdlWindow > other._sdlWindow; }

    bool Window::operator==(Window other) const { return this->_sdlWindow == other._sdlWindow; }

    Window::~Window() {
        _windowTracker.erase(_windowTracker.find(this));

        SDL_GL_DeleteContext(this->_sdlGlContext);
        SDL_DestroyWindow(this->_sdlWindow);
    }

} // namespace glee
