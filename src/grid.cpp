#include "grid.hpp"

#include <vector>
#include <iostream>
#include "complex.hpp"

MandelbrotGrid::MandelbrotGrid()
{
    m_width = 1;
    m_height = 1;
    lowerBound.set(-2.0, -2.0);
    upperBound.set(2.0, 2.0);
    renderedToX = 0;
    renderedToY = 0;
    iterationCount = 0;
    iterationMaximum = 256;
}

void MandelbrotGrid::initializeGrid(int width, int height, double realLowerBound, double imaginaryLowerBound, double realUpperBound, double imaginaryUpperBound)
{
    m_width = width;
    m_height = height;

    lowerBound.set(realLowerBound, imaginaryLowerBound);
    upperBound.set(realUpperBound, imaginaryUpperBound);

    grid.resize(width * height);
    grid.assign(width * height, Complex(0.0, 0.0));

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

Complex MandelbrotGrid::valueAt(int x, int y)
{
    return grid[x * m_height + y];
}

bool MandelbrotGrid::divergesAt(int x, int y)
{
    return valueAt(x, y).magnitude() > 2;
}

int MandelbrotGrid::getIterationCount()
{
    return iterationCount;
}

int MandelbrotGrid::iterationsAt(int x, int y)
{
    return iterationGrid[x * m_height + y];
}

Complex MandelbrotGrid::mapToComplex(double x, double y)
{
    double realRange = upperBound.real - lowerBound.real;
    double imaginaryRange = upperBound.imag - lowerBound.imag;
    x *= realRange / static_cast<double>(m_width - 1);
    y *= imaginaryRange / static_cast<double>(m_height - 1);

    x += lowerBound.real;
    y += lowerBound.imag;

    y = upperBound.imag - (y - lowerBound.imag);

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
    if (iterationCount < iterationMaximum)
    {
        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                if (valueAt(x, y).magnitude() <= 2)
                {
                    setValueAt(
                        x, y,
                        addComplex(valueAt(x, y).squareInplace(), mapToComplex(x, y))
                    );
                    incrementIterationGrid(x, y);
                }
            }
        }
        if (iterationCount == iterationMaximum - 1) std::cout << "finishing up\n";
        iterationCount++;
    }
}
