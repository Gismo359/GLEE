#include "GLEE.hpp"

#include "Log.hpp"

#include <fstream>
#include <sstream>

#define LOG_GL_ERROR {\
    auto error = glGetError();\
    if (error) LOG(WARN, "GL_ERROR: %d -> %s", error, glewGetErrorString(error));\
};

namespace glee {

    // Anonymous namespace to keep variables from cluttering namespaces
    // TODO: Move to source file because there is no need for the user to even see these
    namespace {
        using std::string;

        std::vector<EventHandlerPair> _eventCallbacks;

        // Frame length in milliseconds
        // TODO: Maybe use per-window FPS tracking?
        uint32_t _frameLength;

        bool _loopRunning;

        void ProcessEventHandlers(uint32_t delta, SDL_Event event);
        void ProcessRenderHandlers(uint32_t delta);

        void ProcessEventHandlers(uint32_t delta, SDL_Event event) {
            for (auto&& pair : _eventCallbacks) { pair.first(delta, event, pair.second); }
        }

        // TODO: Move to GL section(s)
        uint32_t _shaderProgram = 0;
        int32_t _uniformProjectionMatrix = 0;
        int32_t _uniformModelViewMatrix = 0;

        void ProcessRenderHandlers(uint32_t delta) {
            // Rendering is called per-window but is scheduled statically
            // This causes some problems but I cant think of a way to avoid it currently

            ///* ^^ I have a really elegant proof of this, but this margins is too narrow *///
            Window::Render(delta);
        }

    } // namespace

    // TODO: Move to proper sections

    std::string readAllLines(std::string path) {
        std::ifstream file(path);
        std::string line;
        std::ostringstream stream;
        while (std::getline(file, line)) {
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

    void compileShaderProgram() {
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
//        LOG_GL_ERROR

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

            LOG(ERROR, "Couldn't validate shaders. %s", infoLogString.c_str());

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

    void AddEventCallback(EventHandler callback, CallbackData data) {
        _eventCallbacks.push_back(std::make_pair(callback, data));
    }

    int Init() {
        auto error = SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        if (error < 0) { LOG(FATAL, "SDL2 was unable to initialise: %s", SDL_GetError()); }

        return error;
    }

    void StartLoop() {
        _loopRunning = true;

        Uint32 lastTick = SDL_GetTicks();

        while (_loopRunning) {
            uint32_t currentTick = SDL_GetTicks();
            uint32_t delta = currentTick - lastTick;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                // TODO: Filter or preprocess some events?
                ProcessEventHandlers(delta, event);
            }

            // TODO: Compromise for multiple missed frames
            if (delta > _frameLength) {
                // TODO: Check for double-buffering and clearing options

                ProcessRenderHandlers(delta);
            }
        }
    }

    void StopLoop() { _loopRunning = false; }

    void Quit() {
        // TODO: Do cleanup (currently not needed)
        StopLoop();
    }

} // namespace glee
