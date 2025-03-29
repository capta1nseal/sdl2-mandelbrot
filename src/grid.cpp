#include "grid.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "complex.hpp"
#include "workqueue.hpp"

MandelbrotGrid::MandelbrotGrid() {
    m_iterationCount = 0;
    m_iterationMaximum = 4096;
    m_escapeRadius = 2.0;
    m_width = 1;
    m_height = 1;
    aspectRatio = static_cast<double>(m_width) / static_cast<double>(m_height);
    m_viewCenter.set(0.0, 0.0);
    m_viewScale = 1.0;
}

void MandelbrotGrid::initializeGrid(int width, int height,
                                    double viewCenterReal,
                                    double viewCenterImag, double viewScale) {
    {
        std::lock_guard<std::mutex> lock(calculationMutex);

        m_viewCenter.set(viewCenterReal, viewCenterImag);
        m_viewScale = viewScale;
    }

    resizeGrid(width, height);
}

void MandelbrotGrid::resizeGrid(int width, int height) {
    std::lock_guard<std::mutex> lock(calculationMutex);

    m_width = width;
    m_height = height;

    aspectRatio = static_cast<double>(m_width) / static_cast<double>(m_height);

    resetGrid();
}

void MandelbrotGrid::resetGrid() {
    workQueue.abortIteration();

    m_grid.resize(m_width * m_height);
    m_grid.assign(m_width * m_height, Complex(0.0, 0.0));

    m_iterationGrid.resize(m_width * m_height);
    m_iterationGrid.assign(m_width * m_height, 0);

    m_magnitudeGrid.resize(m_width * m_height);
    m_magnitudeGrid.assign(m_width * m_height, 0.0);

    m_escapeCount = 0;
    escapeIterationCounter.resize(m_iterationMaximum);
    escapeIterationCounter.assign(m_iterationMaximum, 0);

    m_iterationCount = 0;
}

void MandelbrotGrid::calculationLoop() {
    isRunning = true;
    while (isRunning) {
        iterateGrid();
    }
}

void MandelbrotGrid::stop() { isRunning = false; }

int MandelbrotGrid::getMaxIterationCount() { return m_iterationMaximum; }

void MandelbrotGrid::getFrameData(
    int &iterationCount, int &escapeCount, std::vector<double> &magnitudeGrid,
    std::vector<int> &iterationGrid,
    std::vector<int> &escapeIterationCounterSums) {

    while (m_iterationCount == 0) [[unlikely]] {
    }

    // Loop to keep acquiring mutex until it is acquired after the completion of
    // a non-aborted iteration.
    bool done = false;
    while (!done) {
        std::lock_guard<std::mutex> lock(calculationMutex);

        if (!workQueue.isAborted()) [[likely]] {

            iterationCount = m_iterationCount;

            escapeCount = m_escapeCount;

            magnitudeGrid = m_magnitudeGrid;

            iterationGrid = m_iterationGrid;

            escapeIterationCounterSums.resize(m_iterationMaximum);
            escapeIterationCounterSums[0] = escapeIterationCounter[0];
            for (int i = 1; i < m_iterationMaximum; i++) {
                escapeIterationCounterSums[i] =
                    escapeIterationCounterSums[i - 1] +
                    escapeIterationCounter[i];
            }

            done = true;
        }
    }
}

void MandelbrotGrid::zoomIn(double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewScale *= factor;
    resetGrid();
    printLocation();
}
void MandelbrotGrid::zoomOut(double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewScale /= factor;
    resetGrid();
    printLocation();
}

void MandelbrotGrid::zoomOnPixel(int x, int y) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewCenter.set(mapToComplex(x, y));
    m_viewScale *= 2;
    resetGrid();
    printLocation();
}

void MandelbrotGrid::move(double real, double imag) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewCenter.add(Complex(real / m_viewScale, imag / m_viewScale));
    resetGrid();
    printLocation();
}

void MandelbrotGrid::printLocation() {
    std::cout << "(" << m_viewCenter.real << ", " << m_viewCenter.imag << ", "
              << m_viewScale << ")\n";
}

Complex MandelbrotGrid::mapToComplex(double x, double y) {
    x += 0.5;
    y += 0.5;
    double realRange = (2.0 * m_escapeRadius) / m_viewScale;
    double imaginaryRange = realRange * (static_cast<double>(m_height) /
                                         static_cast<double>(m_width));
    x *= realRange / static_cast<double>(m_width);
    y *= imaginaryRange / static_cast<double>(m_height);

    x += m_viewCenter.real - (m_escapeRadius / m_viewScale);
    y += m_viewCenter.imag - (m_escapeRadius / (m_viewScale * aspectRatio));

    y = 2.0 * m_viewCenter.imag - y;

    return Complex(x, y);
}

void MandelbrotGrid::setValueAt(int x, int y, Complex value) {
    m_grid[x * m_height + y] = value;
}

void MandelbrotGrid::incrementIterationGrid(int x, int y) {
    m_iterationGrid[x * m_height + y] += 1;
}

void MandelbrotGrid::rowIterator() {
    auto [y, width] = workQueue.getTask();

    while (y != -1) {
        // TODO use workQueue's atomic bool to abort before further reads.
        for (int x = 0; x < m_width; x++) {
            if (workQueue.isAborted()) [[unlikely]] {
                break;
            }
            if (m_magnitudeGrid[x * m_height + y] <= m_escapeRadius) {
                m_grid[x * m_height + y].squareAdd(mapToComplex(x, y));
                m_magnitudeGrid[x * m_height + y] =
                    m_grid[x * m_height + y].magnitude();
                incrementIterationGrid(x, y);

                if (m_magnitudeGrid[x * m_height + y] > m_escapeRadius) {
                    m_escapeCount++;
                    escapeIterationCounter[m_iterationGrid[x * m_height + y] -
                                           1]++;
                }
            }
        }

        std::tie(y, width) = workQueue.getTask();
    }
}

void MandelbrotGrid::iterateGrid() {
    if (m_iterationCount < m_iterationMaximum) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        std::lock_guard<std::mutex> lock(calculationMutex);

        workQueue.setTaskCount(m_height);
        workQueue.setTaskLength(m_width);

        unsigned int threadCount = std::thread::hardware_concurrency();
        std::vector<std::jthread> threads;

        {
            for (unsigned int i = 0u; i < threadCount; i++) {
                threads.push_back(
                    std::jthread(&MandelbrotGrid::rowIterator, this));
            }
        }

        if (!workQueue.isAborted()) {
            m_iterationCount++;

            if (m_iterationCount >= m_iterationMaximum) {
                std::cout << "max iteration count reached\n";
            }
        }
    }
}
