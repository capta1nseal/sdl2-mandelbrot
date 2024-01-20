#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <vector>

#include "complex.hpp"

class MandelbrotGrid
{
public:
    MandelbrotGrid();

    void initializeGrid(int width, int height, double realLowerBound, double imaginaryLowerBound, double realUpperBound, double imaginaryUpperBound);

    void tick();

    int width();
    int height();

    Complex valueAt(int x, int y);

    bool divergesAt(int x, int y);

    int getIterationCount();

    int iterationsAt(int x, int y);

private:
    std::vector<Complex> grid;
    std::vector<int> iterationGrid;
    int iterationCount;
    int iterationMaximum;
    int m_width, m_height;
    Complex lowerBound, upperBound;
    int renderedToX, renderedToY;

    Complex mapToComplex(double x, double y);

    void setValueAt(int x, int y, Complex value);

    void incrementIterationGrid(int x, int y);

    void iterateGrid();

};

#endif
