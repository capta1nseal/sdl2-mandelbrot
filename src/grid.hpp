#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <vector>

#include "complex.hpp"

class MandelbrotGrid
{
public:
    MandelbrotGrid();

    void initializeGrid(int width, int height, double viewCenterReal, double viewCenterImag, double viewScale);

    void resizeGrid(int width, int height);

    void resetGrid();

    void tick();

    int width();
    int height();

    double getViewScale();

    Complex valueAt(int x, int y);

    bool divergesAt(int x, int y);

    int getIterationCount();

    int getEscapeCount();

    int getMaxIterationCount();

    int getEscapeIterationCounter(int i);

    int getEscapeIterationCounterSum(int i);
    double getEscapeIterationCounterSum(double i);

    double getEscapeRadius();

    int iterationsAt(int x, int y);

    void zoomIn(double factor);
    void zoomOut(double factor);

    void zoomOnPixel(int x, int y);

    void move(double real, double imag);

private:
    std::vector<Complex> grid;
    std::vector<int> iterationGrid;
    std::vector<int> escapeIterationCounter;
    std::vector<int> escapeIterationCounterSums;
    int m_escapeCount;
    int m_iterationCount;
    int m_iterationMaximum;
    double m_escapeRadius;
    int m_width, m_height;
    double aspectRatio;
    Complex m_viewCenter;
    double m_viewScale;

    Complex mapToComplex(double x, double y);

    void setValueAt(int x, int y, Complex value);

    void incrementIterationGrid(int x, int y);

    void iterateGrid();

};

#endif
