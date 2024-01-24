#ifndef _MANDELBROTAPPLICATION
#define _MANDELBROTAPPLICATION

#include <chrono>

#include <SDL2/SDL.h>

#include "grid.hpp"
#include "shading.hpp"

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

    SDL_Texture *renderTexture;
    unsigned char* texturePixels;
    int texturePitch;

    SDL_Event event;
    const uint8_t *keyboardState;

    SDL_Point mousePosition;

    MandelbrotGrid mandelbrotGrid; 

    Shading shading;
    
    void initializeSdl();
    void destroySdl();

    void initializeGrid();

    void initializeShading();

    void initializeRenderTexture();

    void handleEvents();

    void tick();

    void draw();
};

#endif
