#ifndef _MANDELBROTAPPLICATION
#define _MANDELBROTAPPLICATION

#include <chrono>
#include <thread>

#include <SDL2/SDL.h>

#include "grid.hpp"
#include "shading.hpp"

std::chrono::_V2::steady_clock::time_point now();

// Wrapper class for the application.
// Due to being lazy, holds the code for SDL2.
class MandelbrotApplication {
public:
    MandelbrotApplication();

    void run();

private:
    bool isRunning;
    int frameCounter;
    double timeCounter;
    unsigned int displayWidth, displayHeight;
    bool isFullscreen;

    SDL_DisplayMode displayMode;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *renderTexture;
    unsigned char *texturePixels;
    int texturePitch;

    SDL_Event event;
    const uint8_t *keyboardState;

    SDL_Point mousePosition;

    MandelbrotGrid mandelbrotGrid;
    std::thread calculationThread;

    Shading shading;

    void initializeSdl();
    void destroySdl();

    void initializeGrid();

    void initializeShading();

    void initializeRenderTexture();

    void handleEvents();

    void draw();
};

#endif
