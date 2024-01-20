#include "application.hpp"

#include <chrono>
#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

std::chrono::_V2::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

MandelbrotApplication::MandelbrotApplication()
{
    initializeSdl();

    initializeGrid();

    isRunning = false;
    isFullscreen = false;
}

void MandelbrotApplication::run()
{
    auto start = now();

    std::chrono::duration<double> delta;

    int counter = 0;

    isRunning = true;
    draw();

    while (isRunning)
    {
        start = now();

        handleEvents();

        tick();

        draw();

        delta = now() - start;

        if (counter % 600 == 0)
            std::cout << delta.count() << "\n";
        
        counter += 1;
    }

    destroySdl();
}

void MandelbrotApplication::initializeSdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GetCurrentDisplayMode(0, &displayMode);

    displayWidth = displayMode.w / 2;
    displayHeight = displayMode.h / 2;

    uint32_t windowFlage = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow(
        "mandelbrot",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        displayWidth, displayHeight,
        windowFlage);
    
    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    keyboardState = SDL_GetKeyboardState(NULL);
}
void MandelbrotApplication::destroySdl()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MandelbrotApplication::initializeGrid()
{
    mandelbrotGrid.initializeGrid(960, 540, -2.0, -2.0 * 9.0 / 16.0, 2.0, 2.0 * 9.0 / 16.0);
}

void MandelbrotApplication::handleEvents()
{
    SDL_PumpEvents();
    
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                displayWidth = event.window.data1;
                displayHeight = event.window.data2;

                double scaleFactor = static_cast<double>(displayHeight) / static_cast<double>(displayWidth);
                mandelbrotGrid.initializeGrid(displayWidth, displayHeight, -2.0, -2.0 * scaleFactor, 2.0, 2.0 * scaleFactor);
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_F11:
                if (isFullscreen)
                {
                    SDL_SetWindowFullscreen(window, 0);
                }
                else
                {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                isFullscreen = !isFullscreen;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                break;
            }
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void MandelbrotApplication::tick()
{
    mandelbrotGrid.tick();
}

void MandelbrotApplication::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int width = mandelbrotGrid.width();
    int height = mandelbrotGrid.height();
    int iterationCount = mandelbrotGrid.getIterationCount();
    double colourFactor = 0;
    Uint8 alpha = 0;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (mandelbrotGrid.divergesAt(x, y))
            {
                colourFactor = (static_cast<double>(mandelbrotGrid.iterationsAt(x, y) + 1) - (log(log(mandelbrotGrid.valueAt(x, y).magnitude())) / log(2.0))) / iterationCount;
                colourFactor = colourFactor * (2.0 - colourFactor); // remap the factor, grouping more high values near the boundary
                alpha = static_cast<int>(colourFactor * 255.0);
                SDL_SetRenderDrawColor(renderer, alpha, alpha/2, alpha/8, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
