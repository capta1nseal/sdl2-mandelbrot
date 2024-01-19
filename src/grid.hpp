#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <vector>

#include "complex.hpp"

class MandelbrotGrid
{
public:
    std::vector<Complex> grid;

    MandelbrotGrid();

    void initializeGrid(int width, int height);

    void tick();

    Complex valueAt(int x, int y);

    void testFunction();

private:
    int m_width, m_height;
    int renderedToX, renderedToY;

    Complex mapToComplexPlane(double x, double y);

    void setValueAt(int x, int y, Complex value);

    void iterateGrid();

};

#endif
