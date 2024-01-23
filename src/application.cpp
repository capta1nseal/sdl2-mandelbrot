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

    uint32_t windowFlags = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow(
        "mandelbrot",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        displayWidth, displayHeight,
        windowFlags);
    
    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    keyboardState = SDL_GetKeyboardState(NULL);
}
void MandelbrotApplication::destroySdl()
{
    SDL_DestroyTexture(renderTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MandelbrotApplication::initializeGrid()
{
    mandelbrotGrid.initializeGrid(displayWidth, displayHeight, 0.0, 0.0, 1.0);

    // mandelbrotGrid.initializeGrid(displayWidth, displayHeight, -0.747089, 0.100153, 955.594); // - takes you to a zoom in seahorse valley

    initializeRenderTexture();
}

void MandelbrotApplication::initializeRenderTexture()
{
    renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, displayWidth, displayHeight);
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

                mandelbrotGrid.resizeGrid(displayWidth, displayHeight);

                initializeRenderTexture();
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
            case SDL_SCANCODE_UP:
                mandelbrotGrid.zoomIn(1.1);
                initializeRenderTexture();
                break;
            case SDL_SCANCODE_DOWN:
                mandelbrotGrid.zoomOut(1.1);
                initializeRenderTexture();
                break;
            case SDL_SCANCODE_W:
                mandelbrotGrid.move(0.0, 0.1);
                initializeRenderTexture();
                break;
            case SDL_SCANCODE_S:
                mandelbrotGrid.move(0.0, -0.1);
                initializeRenderTexture();
                break;
            case SDL_SCANCODE_A:
                mandelbrotGrid.move(-0.1, 0.0);
                initializeRenderTexture();
                break;
            case SDL_SCANCODE_D:
                mandelbrotGrid.move(0.1, 0.0);
                initializeRenderTexture();
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
                mandelbrotGrid.zoomOnPixel(event.button.x, event.button.y);
                initializeRenderTexture();
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
    if (mandelbrotGrid.getIterationCount() == 0)
    {
        do
        {
            mandelbrotGrid.tick();
        } while (mandelbrotGrid.getEscapeCount() == 0 and mandelbrotGrid.getIterationCount() < 16);
        return;
    }

    mandelbrotGrid.tick();
}

void MandelbrotApplication::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int width = mandelbrotGrid.width();
    int height = mandelbrotGrid.height();
    // double iterationCount = static_cast<double>(mandelbrotGrid.getIterationCount());
    double escapeCount = mandelbrotGrid.getEscapeCount();
    double escapeIterationCount;
    double localValueMagnitude;
    double colourFactor = 0;
    Uint8 alpha = 0;

    SDL_LockTexture(renderTexture, NULL, (void**)&texturePixels, &texturePitch);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (mandelbrotGrid.divergesAt(x, y))
            {
                escapeIterationCount = mandelbrotGrid.iterationsAt(x, y);
                localValueMagnitude = mandelbrotGrid.valueAt(x, y).magnitude();
                // calculate continuous number of iterations to escape
                escapeIterationCount = (escapeIterationCount - log2(log2(localValueMagnitude)));
                // get Lerped summed histogram for continuous histogram shading
                colourFactor = mandelbrotGrid.getEscapeIterationCounterSum(escapeIterationCount) / escapeCount;

                alpha = static_cast<int>((1.0 - colourFactor) * 255.0);

                texturePixels[y * texturePitch + x * 4] = (unsigned char)alpha;
                texturePixels[y * texturePitch + x * 4 + 1] = (unsigned char)alpha;
                texturePixels[y * texturePitch + x * 4 + 2] = (unsigned char)alpha;
                texturePixels[y * texturePitch + x * 4 + 3] = (unsigned char)255;
            }

            // else
            // {
            //     texturePixels[y * texturePitch + x * 4] = (unsigned char)0;
            //     texturePixels[y * texturePitch + x * 4 + 1] = (unsigned char)0;
            //     texturePixels[y * texturePitch + x * 4 + 2] = (unsigned char)0;
            //     texturePixels[y * texturePitch + x * 4 + 3] = (unsigned char)255;
            // }
        }
    }

    SDL_UnlockTexture(renderTexture);

    SDL_RenderCopy(renderer, renderTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
}
