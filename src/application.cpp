#include "application.hpp"

#include <chrono>
#include <iostream>

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


    mandelbrotGrid.testFunction();

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
    mandelbrotGrid.initializeGrid(349, 174);
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
                break;
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
    // do update stuff
}

void MandelbrotApplication::draw()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // do drawing stuff

    SDL_RenderPresent(renderer);
}
