#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <vector>

#include "complex.hpp"

class MandelbrotGrid
{
public:
    std::vector<Complex> grid;

    MandelbrotGrid();

    void initializeGrid(int width, int height, double realLowerBound, double imaginaryLowerBound, double realUpperBound, double imaginaryUpperBound);

    void tick();

    int width();
    int height();

    Complex valueAt(int x, int y);

    bool divergesAt(int x, int y);

private:
    int m_width, m_height;
    Complex lowerBound, upperBound;
    int renderedToX, renderedToY;

    Complex mapToComplexPlane(double x, double y);

    void setValueAt(int x, int y, Complex value);

    void iterateGrid();

};

#endif
