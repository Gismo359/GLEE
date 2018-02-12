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
    }

    Window::Window(const std::string& title, int x, int y, int width, int height) {
        if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
            LOG(ERROR, "SDL Failed to initialize!");
            abort();
        }

        _sdlWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
        auto context = SDL_GL_CreateContext(_sdlWindow);

        glewExperimental = true;
        auto error = glewInit();

        if (error) {
            LOG(INFO, "%s", glewGetErrorString(error));
        }

        compileShaderProgram();
        LOG(INFO, "%s -> %s", glGetString(GL_VENDOR), glGetString(GL_VERSION));
    }

    void Window::loop() {
        _running = true;

        SDL_GL_SwapWindow(_sdlWindow);
        Uint32 lastTick = SDL_GetTicks();

        while (_running) {
            Uint32 currentTick = SDL_GetTicks();
            Uint32 delta = currentTick - lastTick;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                // TODO: Filter or preprocess some events?
                callEventCallbacks(delta, event);
            }

            // TODO: Comprimise for multiple missed frames
            if (delta > getFrameLength()) {
                // TODO: Check for double-buffering and clearing options

                glUseProgram(_shaderProgram);
                LOG_GL_ERROR

                _uniformProjectionMatrix = glGetUniformLocation(_shaderProgram, "projectionMatrix");
                _uniformModelViewMatrix = glGetUniformLocation(_shaderProgram, "modelViewMatrix");

                float matrixIdentity[]{
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                };

                glUniformMatrix4fv(_uniformProjectionMatrix, 1, GL_FALSE, matrixIdentity);
                LOG_GL_ERROR
                glUniformMatrix4fv(_uniformModelViewMatrix, 1, GL_FALSE, matrixIdentity);
                LOG_GL_ERROR

                callRenderCallbacks(delta);
                LOG_GL_ERROR

                SDL_GL_SwapWindow(_sdlWindow);
            }
        }
    }

    // TODO: Maybe add separate wrappers for width/height
    void Window::getSize(int& width, int& height) const { SDL_GetWindowSize(_sdlWindow, &width, &height); }

    void Window::setSize(int width, int height) { SDL_SetWindowSize(_sdlWindow, width, height); }

    Uint32 Window::getFrameLength() const { return _frameLength; }

    void Window::setFrameLength(Uint32 newLength) { _frameLength = newLength; }

    std::string Window::getTitle() const { return SDL_GetWindowTitle(_sdlWindow); }

    void Window::setTitle(const std::string& title) { SDL_SetWindowTitle(_sdlWindow, title.c_str()); }

    void Window::addRenderCallback(Window::RenderCallback callback) { _renderCallbacks.push_back(callback); }

    void Window::addEventCallback(Window::EventCallback callback) { _eventCallbacks.push_back(callback); }

    void Window::stop() {
        // TODO: Do render-specific cleanup if necessary
        _running = false;
    }

    void Window::close() {
        stop();
        SDL_DestroyWindow(_sdlWindow);
        SDL_Quit();
    }
} // namespace glee
