//
// Created by braynstorm on 2/10/18.
//

#ifndef GLEE_GLEE_HPP
#define GLEE_GLEE_HPP

#include "Utils.hpp"
#include "Window.hpp"
#include "drawing/Line.hpp"
#include "drawing/LineBatch.hpp"

namespace glee {

    int Init();

    void AddEventCallback(EventHandler handler, CallbackData data);

    void ClearRenderCallbacks(Window &window);

    // TODO: Maybe add options when starting/stopping?
    void StartLoop();
    void StopLoop();

    // TODO: Maybe add pausing logic for rendering?
    // void pause();

    // Temporary shader compilation
    // TODO: Move to proper section
    void compileShaderProgram();

    void Quit();
} // namespace glee

#endif // GLEE_GLEE_HPP
