#include "grid.hpp"

#include <vector>
#include <iostream>
#include <numeric>
#include "complex.hpp"

MandelbrotGrid::MandelbrotGrid()
{
    m_iterationCount = 0;
    m_iterationMaximum = 4096;
    m_escapeRadius = 2.0;
    m_width = 1;
    m_height = 1;
    aspectRatio = static_cast<double>(m_width) / static_cast<double>(m_height);
    m_viewCenter.set(0.0, 0.0);
    m_viewScale = 1.0;
}

void MandelbrotGrid::initializeGrid(int width, int height, double viewCenterReal, double viewCenterImag, double viewScale)
{
    m_viewCenter.set(viewCenterReal, viewCenterImag);
    m_viewScale = viewScale;

    resizeGrid(width, height);
}

void MandelbrotGrid::resizeGrid(int width, int height)
{
    m_width = width;
    m_height = height;

    aspectRatio = static_cast<double>(m_width) / static_cast<double>(m_height);

    resetGrid();
}

void MandelbrotGrid::resetGrid()
{
    grid.resize(m_width * m_height);
    grid.assign(m_width * m_height, Complex(0.0, 0.0));

    iterationGrid.resize(m_width * m_height);
    iterationGrid.assign(m_width * m_height, 0);

    m_escapeCount = 0;
    escapeIterationCounter.resize(m_iterationMaximum);
    escapeIterationCounter.assign(m_iterationMaximum, 0);
    escapeIterationCounterSums.resize(m_iterationMaximum);
    escapeIterationCounterSums.resize(m_iterationMaximum, 0);

    m_iterationCount = 0;
}

void MandelbrotGrid::tick()
{
    iterateGrid();
}

int MandelbrotGrid::width()
{
    return m_width;
}
int MandelbrotGrid::height()
{
    return m_height;
}

Complex MandelbrotGrid::valueAt(int x, int y)
{
    return grid[x * m_height + y];
}

bool MandelbrotGrid::divergesAt(int x, int y)
{
    return valueAt(x, y).magnitude() > m_escapeRadius;
}

int MandelbrotGrid::getIterationCount()
{
    return m_iterationCount;
}

int MandelbrotGrid::getEscapeCount()
{
    return m_escapeCount;
}

int MandelbrotGrid::getEscapeIterationSum(int i)
{
    return escapeIterationCounterSums[i];
}

double MandelbrotGrid::getEscapeRadius()
{
    return m_escapeRadius;
}

int MandelbrotGrid::iterationsAt(int x, int y)
{
    return iterationGrid[x * m_height + y];
}

void MandelbrotGrid::zoomIn(double factor)
{
    m_viewScale *= factor;
    resetGrid();
    std::cout << m_viewScale << "\n";
}
void MandelbrotGrid::zoomOut(double factor)
{
    m_viewScale /= factor;
    resetGrid();
    std::cout << m_viewScale << "\n";
}

void MandelbrotGrid::move(double real, double imag)
{
    m_viewCenter.add(Complex(real / m_viewScale, imag / m_viewScale));
    resetGrid();
    std::cout << "(" << m_viewCenter.real << "," << m_viewCenter.imag << ")\n";
}

Complex MandelbrotGrid::mapToComplex(double x, double y)
{
    double realRange = (2.0 * m_escapeRadius) / m_viewScale;
    double imaginaryRange = realRange * (static_cast<double>(m_height) / static_cast<double>(m_width));
    x *= realRange / static_cast<double>(m_width - 1);
    y *= imaginaryRange / static_cast<double>(m_height - 1);

    x += m_viewCenter.real - (m_escapeRadius / m_viewScale);
    y += m_viewCenter.imag - (m_escapeRadius / (m_viewScale * aspectRatio));

    y = 2.0 * m_viewCenter.imag - y;

    return Complex(x, y);
}

void MandelbrotGrid::setValueAt(int x, int y, Complex value)
{
    grid[x * m_height + y] = value;
}

void MandelbrotGrid::incrementIterationGrid(int x, int y)
{
    iterationGrid[x * m_height + y] += 1;
}

void MandelbrotGrid::iterateGrid()
{
    if (m_iterationCount < m_iterationMaximum)
    {
        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                if (valueAt(x, y).magnitude() <= m_escapeRadius)
                {
                    grid[x * m_height + y].squaredPlus(mapToComplex(x, y));
                    incrementIterationGrid(x, y);

                    if (valueAt(x, y).magnitude() > m_escapeRadius)
                    {
                        m_escapeCount++;
                        escapeIterationCounter[iterationsAt(x, y)]++;
                    }
                }
            }
        }

        for (int i = 0; i < m_iterationMaximum; i++)
        {
            escapeIterationCounterSums[i] = std::accumulate(escapeIterationCounter.begin(), escapeIterationCounter.begin() + i + 1, 0);
        }

        if (m_iterationCount == m_iterationMaximum - 1) std::cout << "finishing up\n";
        m_iterationCount++;
    }
}
