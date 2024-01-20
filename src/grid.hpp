#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <vector>

#include <complex>

class MandelbrotGrid
{
public:
    MandelbrotGrid();

    void initializeGrid(int width, int height, double realLowerBound, double imaginaryLowerBound, double realUpperBound, double imaginaryUpperBound);

    void tick();

    int width();
    int height();

    std::complex<double> valueAt(int x, int y);

    bool divergesAt(int x, int y);

    int getIterationCount();

    int iterationsAt(int x, int y);

private:
    std::vector<std::complex<double>> grid;
    std::vector<int> iterationGrid;
    int iterationCount;
    int iterationMaximum;
    int m_width, m_height;
    std::complex<double> lowerBound, upperBound;
    int renderedToX, renderedToY;

    std::complex<double> mapToComplex(double x, double y);

    void setValueAt(int x, int y, std::complex<double> value);

    void incrementIterationGrid(int x, int y);

    void iterateGrid();

};

#endif
