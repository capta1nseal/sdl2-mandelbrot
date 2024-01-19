#include "grid.hpp"

#include <vector>
#include <iostream>

#include "complex.hpp"

MandelbrotGrid::MandelbrotGrid()
{
    m_width = 1;
    m_height = 1;
    renderedToX = 0;
    renderedToY = 0;
}

void MandelbrotGrid::initializeGrid(int width, int height)
{
    m_width = width;
    m_height = height;
    grid.resize(width * height);
}

void MandelbrotGrid::tick()
{

}

Complex MandelbrotGrid::valueAt(int x, int y)
{
    return grid[x * m_height + y];
}

void MandelbrotGrid::testFunction()
{
    for (int i = 0; i < 10000; i++)
    {
        iterateGrid();
    }

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            if (valueAt(x, y).magnitude() <= 2)
            {
                std::cout << " ";
            }
            else
            {
                std::cout << "#";
            }
        }
        std::cout << "\n";
    }
    
}

Complex MandelbrotGrid::mapToComplexPlane(double x, double y)
{
    x *= 4.0 / static_cast<double>(m_width - 1);
    y *= 4.0 / static_cast<double>(m_height - 1);

    x -= 2.0;
    y -= 2.0;

    y *= -1.0;

    return Complex(x, y);
}

void MandelbrotGrid::setValueAt(int x, int y, Complex value)
{
    grid[x * m_height + y] = value;
}

void MandelbrotGrid::iterateGrid()
{
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            if (valueAt(x, y).magnitude() <= 2)
            {
                setValueAt(
                    x, y,
                    addComplex(grid[x * m_height + y].squareInplace(), mapToComplexPlane(x, y))
                );
            }
        }
    }
}
