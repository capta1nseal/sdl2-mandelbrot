#ifndef _MANDELBROTAPPLICATION
#define _MANDELBROTAPPLICATION

#include <chrono>

#include <SDL2/SDL.h>

#include "grid.hpp"

std::chrono::_V2::steady_clock::time_point now();

class MandelbrotApplication
{
public:
    MandelbrotApplication();

    void run();

private:
    bool isRunning;
    unsigned int displayWidth, displayHeight;
    bool isFullscreen;

    SDL_DisplayMode displayMode;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Event event;
    const uint8_t *keyboardState;

    SDL_Point mousePosition;

    MandelbrotGrid mandelbrotGrid; 
    
    void initializeSdl();
    void destroySdl();

    void initializeGrid();

    void handleEvents();

    void tick();

    void draw();
};

#endif
