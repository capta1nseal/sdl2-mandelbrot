#include "application.hpp"

#include <SDL2/SDL_render.h>
#include <chrono>
#include <cmath>
#include <thread>

#include <SDL2/SDL.h>

#include "grid.hpp"
#include "shading.hpp"

std::chrono::_V2::steady_clock::time_point now() {
    return std::chrono::steady_clock::now();
}

MandelbrotApplication::MandelbrotApplication() {
    initializeSdl();
    initializeGrid();
    initializeShading();

    isRunning = false;
    isFullscreen = false;
}

void MandelbrotApplication::run() {
    auto start = now();

    std::chrono::duration<double> delta;

    int frameCounter = 0;

    calculationThread =
        std::thread(&MandelbrotGrid::calculationLoop, &mandelbrotGrid);

    isRunning = true;
    draw();

    while (isRunning) {
        start = now();

        handleEvents();

        draw();

        delta = now() - start;

        frameCounter += 1;
    }

    mandelbrotGrid.stop();

    calculationThread.join();

    destroySdl();
}

void MandelbrotApplication::initializeSdl() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GetCurrentDisplayMode(0, &displayMode);

    displayWidth = displayMode.w / 2;
    displayHeight = displayMode.h / 2;

    // Testing small window size, useful for valgrind.
    // displayWidth = 160;
    // displayHeight = 90;

    uint32_t windowFlags = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow("mandelbrot", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, displayWidth,
                              displayHeight, windowFlags);

    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    keyboardState = SDL_GetKeyboardState(NULL);
}
void MandelbrotApplication::destroySdl() {
    SDL_DestroyTexture(renderTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MandelbrotApplication::initializeGrid() {
    mandelbrotGrid.initializeGrid(displayWidth, displayHeight, 0.0, 0.0, 1.0);

    // random test location
    // mandelbrotGrid.initializeGrid(displayWidth, displayHeight, 0.260224,
    // -0.00184122, 2998.48);

    // a zoom in seahorse valley
    // mandelbrotGrid.initializeGrid(displayWidth, displayHeight, -0.747089,
    // 0.100153, 955.594);

    // testing spot for high-end of values
    // mandelbrotGrid.initializeGrid(displayWidth, displayHeight, 0.172403,
    // 0.563459, 8192);

    // testing spot for very high detail and iteration count
    // mandelbrotGrid.initializeGrid(displayWidth, displayHeight, 0.330646,
    // -0.39128, 46736.3);

    initializeRenderTexture();
}

void MandelbrotApplication::initializeShading() {
    shading.setShadingFunction(3);
}

void MandelbrotApplication::initializeRenderTexture() {
    SDL_DestroyTexture(renderTexture);
    renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING, displayWidth,
                                      displayHeight);
}

void MandelbrotApplication::handleEvents() {
    SDL_PumpEvents();

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                displayWidth = event.window.data1;
                displayHeight = event.window.data2;

                mandelbrotGrid.resizeGrid(displayWidth, displayHeight);

                initializeRenderTexture();
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_F11:
                if (isFullscreen) {
                    SDL_SetWindowFullscreen(window, 0);
                } else {
                    SDL_SetWindowFullscreen(window,
                                            SDL_WINDOW_FULLSCREEN_DESKTOP);
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
            case SDL_SCANCODE_1:
                shading.setShadingFunction(0);
                break;
            case SDL_SCANCODE_2:
                shading.setShadingFunction(1);
                break;
            case SDL_SCANCODE_3:
                shading.setShadingFunction(2);
                break;
            case SDL_SCANCODE_4:
                shading.setShadingFunction(3);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
            case SDL_BUTTON_LEFT: {
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

void MandelbrotApplication::draw() {
    int iterationCount;
    int escapeCount;
    std::vector<double> magnitudeGrid;
    std::vector<int> iterationGrid;
    std::vector<int> escapeIterationCounterSums;

    mandelbrotGrid.getFrameData(iterationCount, escapeCount, magnitudeGrid,
                                iterationGrid, escapeIterationCounterSums);

    auto smoothEscapeIterationCounterSum =
        [escapeIterationCounterSums](
            const double escapeIterationCount) -> double {
        int a = std::max(static_cast<int>(floor(escapeIterationCount)), 0);

        int b =
            std::min(static_cast<int>(ceil(escapeIterationCount)),
                     static_cast<int>(escapeIterationCounterSums.size()) - 1);

        if (b <= a)
            return escapeIterationCounterSums[static_cast<int>(
                escapeIterationCount)];

        double i = static_cast<double>(escapeIterationCount - a) /
                   static_cast<double>(b - a);
        return escapeIterationCounterSums[a] +
               i * (escapeIterationCounterSums[b] -
                    escapeIterationCounterSums[a]);
    };

    double escapeIterationCount;
    double histogramFactor;
    Shading::Colour colour;

    colour = shading.shade(1.0);
    SDL_SetRenderDrawColor(renderer, get<0>(colour), get<1>(colour),
                           get<2>(colour), 255);
    SDL_RenderClear(renderer);

    SDL_LockTexture(renderTexture, NULL,
                    reinterpret_cast<void **>(&texturePixels), &texturePitch);

    for (unsigned int x = 0; x < displayWidth; x++) {
        for (unsigned int y = 0; y < displayHeight; y++) {
            if (magnitudeGrid[x * displayHeight + y] > 2.0) {
                // calculate continuous number of iterations to escape
                escapeIterationCount =
                    (iterationGrid[x * displayHeight + y] -
                     log2(log2(magnitudeGrid[x * displayHeight + y])));
                // get Lerped summed histogram for continuous histogram shading
                histogramFactor = smoothEscapeIterationCounterSum(
                                      escapeIterationCount - 1.0) /
                                  static_cast<double>(escapeCount);

                colour = shading.shade(histogramFactor);

                texturePixels[y * texturePitch + x * 4] =
                    static_cast<unsigned char>(get<2>(colour));
                texturePixels[y * texturePitch + x * 4 + 1] =
                    static_cast<unsigned char>(get<1>(colour));
                texturePixels[y * texturePitch + x * 4 + 2] =
                    static_cast<unsigned char>(get<0>(colour));
                texturePixels[y * texturePitch + x * 4 + 3] =
                    static_cast<unsigned char>(255);
            }
        }
    }

    SDL_UnlockTexture(renderTexture);

    SDL_SetTextureBlendMode(renderTexture, SDL_BLENDMODE_BLEND);

    SDL_RenderCopy(renderer, renderTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
}
