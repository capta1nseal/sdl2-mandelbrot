#include "grid.hpp"

#include <vector>
#include <iostream>
#include <complex>

MandelbrotGrid::MandelbrotGrid()
{
    using namespace std::complex_literals;
    m_width = 1;
    m_height = 1;
    lowerBound = - 2.0 - 2.0i;
    upperBound = 2.0 + 2.0i;
    renderedToX = 0;
    renderedToY = 0;
    iterationCount = 0;
    iterationMaximum = 256;
}

void MandelbrotGrid::initializeGrid(int width, int height, double realLowerBound, double imaginaryLowerBound, double realUpperBound, double imaginaryUpperBound)
{
    using namespace std::complex_literals;

    m_width = width;
    m_height = height;

    lowerBound = realLowerBound + imaginaryLowerBound*1i;
    upperBound = realUpperBound + imaginaryUpperBound*1i;

    grid.resize(width * height);
    grid.assign(width * height, std::complex<double>(0.0, 0.0));

    iterationGrid.resize(width * height);
    iterationGrid.assign(width * height, 0);

    iterationCount = 0;
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

std::complex<double> MandelbrotGrid::valueAt(int x, int y)
{
    return grid[x * m_height + y];
}

bool MandelbrotGrid::divergesAt(int x, int y)
{
    return abs(valueAt(x, y)) > 2;
}

int MandelbrotGrid::getIterationCount()
{
    return iterationCount;
}

int MandelbrotGrid::iterationsAt(int x, int y)
{
    return iterationGrid[x * m_height + y];
}

std::complex<double> MandelbrotGrid::mapToComplex(double x, double y)
{
    double realRange = upperBound.real() - lowerBound.real();
    double imaginaryRange = upperBound.imag() - lowerBound.imag();
    x *= realRange / static_cast<double>(m_width - 1);
    y *= imaginaryRange / static_cast<double>(m_height - 1);

    x += lowerBound.real();
    y += lowerBound.imag();

    y = upperBound.imag() - (y - lowerBound.imag());

    return std::complex<double>(x, y);
}

void MandelbrotGrid::setValueAt(int x, int y, std::complex<double> value)
{
    grid[x * m_height + y] = value;
}

void MandelbrotGrid::incrementIterationGrid(int x, int y)
{
    iterationGrid[x * m_height + y] += 1;
}

void MandelbrotGrid::iterateGrid()
{
    if (iterationCount < iterationMaximum)
    {
        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                if (abs(valueAt(x, y)) <= 2)
                {
                    setValueAt(
                        x, y,
                        valueAt(x, y) * valueAt(x, y) + mapToComplex(x, y)
                    );
                    incrementIterationGrid(x, y);
                }
            }
        }
        if (iterationCount == iterationMaximum - 1) std::cout << "finishing up\n";
        iterationCount++;
    }
}
