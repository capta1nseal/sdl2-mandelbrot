#include "grid.hpp"

#include "complex.hpp"
#include <cmath>
#include <iostream>
#include <mutex>
#include <vector>

MandelbrotGrid::MandelbrotGrid() {
    m_iterationCount = 0;
    safe_iterationCount = 0;
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
    invalidateCurrentIteration = true;

    grid.clear();
    grid.resize(m_width * m_height);
    grid.assign(m_width * m_height, Complex(0.0, 0.0));

    safe_magnitudeGrid.clear();
    safe_magnitudeGrid.resize(m_width * m_height);
    safe_magnitudeGrid.assign(m_width * m_height, 0.0);

    m_iterationGrid.clear();
    m_iterationGrid.resize(m_width * m_height);
    m_iterationGrid.assign(m_width * m_height, 0);

    safe_iterationGrid.clear();
    safe_iterationGrid.resize(m_width * m_height);
    safe_iterationGrid.assign(m_width * m_height, 0);

    m_escapeCount = 0;
    safe_escapeCount = 0;
    escapeIterationCounter.clear();
    escapeIterationCounter.resize(m_iterationMaximum);
    escapeIterationCounter.assign(m_iterationMaximum, 0);
    safe_escapeIterationCounterSums.clear();
    safe_escapeIterationCounterSums.resize(m_iterationMaximum);
    safe_escapeIterationCounterSums.resize(m_iterationMaximum, 0);

    m_iterationCount = 0;
    safe_iterationCount = 0;
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
    std::lock_guard<std::mutex> lock(calculationMutex);

    iterationCount = safe_iterationCount;

    escapeCount = safe_escapeCount;

    magnitudeGrid = safe_magnitudeGrid;

    iterationGrid = safe_iterationGrid;

    escapeIterationCounterSums = safe_escapeIterationCounterSums;
}

void MandelbrotGrid::zoomIn(double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewScale *= factor;
    resetGrid();
    std::cout << m_viewScale << "\n";
}
void MandelbrotGrid::zoomOut(double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewScale /= factor;
    resetGrid();
    std::cout << m_viewScale << "\n";
}

void MandelbrotGrid::zoomOnPixel(int x, int y) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewCenter.set(mapToComplex(x, y));
    m_viewScale *= 2;
    resetGrid();
}

void MandelbrotGrid::move(double real, double imag) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewCenter.add(Complex(real / m_viewScale, imag / m_viewScale));
    resetGrid();
    std::cout << "(" << m_viewCenter.real << "," << m_viewCenter.imag << ")\n";
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
    grid[x * m_height + y] = value;
}

void MandelbrotGrid::incrementIterationGrid(int x, int y) {
    m_iterationGrid[x * m_height + y] += 1;
}

void MandelbrotGrid::iterateGrid() {

    if (m_iterationCount < m_iterationMaximum) {
        {
            std::lock_guard<std::mutex> lock(calculationMutex);

            invalidateCurrentIteration = false;
        }
        for (int x = 0; x < m_width; x++) {
            for (int y = 0; y < m_height; y++) {
                if (grid[x * m_height + y].magnitudeSquared() <=
                    (m_escapeRadius * m_escapeRadius)) {
                    grid[x * m_height + y].squareAdd(mapToComplex(x, y));
                    incrementIterationGrid(x, y);

                    if (grid[x * m_height + y].magnitudeSquared() >
                        (m_escapeRadius * m_escapeRadius)) {
                        m_escapeCount++;
                        escapeIterationCounter
                            [m_iterationGrid[x * m_height + y] - 1]++;
                    }
                }
            }
            if (invalidateCurrentIteration)
                break;
        }

        if (not invalidateCurrentIteration) { // update data once full iteration
                                              // has been processed to be
                                              // accessed by renderer
            std::lock_guard<std::mutex> lock(calculationMutex);

            safe_escapeCount = m_escapeCount;

            for (int i = 0; i < m_width * m_height; i++) {
                safe_magnitudeGrid[i] = grid[i].magnitude();
                safe_iterationGrid[i] = m_iterationGrid[i];
            }

            safe_escapeIterationCounterSums[0] = escapeIterationCounter[0];
            for (int i = 1; i < m_iterationMaximum; i++) {
                safe_escapeIterationCounterSums[i] =
                    safe_escapeIterationCounterSums[i - 1] +
                    escapeIterationCounter[i];
            }
            m_iterationCount++;
            safe_iterationCount++;
        }

        if (m_iterationCount >= m_iterationMaximum) {
            std::cout << "max iteration count reached\n";
        }
    }
}
