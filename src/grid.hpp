#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <mutex>
#include <vector>

#include "complex.hpp"
#include "workqueue.hpp"

// Wrapper for data and number crunching for the fractal solver.
class MandelbrotGrid {
public:
    MandelbrotGrid();

    void initializeGrid(int width, int height, double viewCenterReal,
                        double viewCenterImag, double viewScale);

    void resizeGrid(int width, int height);

    void resetGrid();

    void calculationLoop();

    void stop();

    int getMaxIterationCount();

    void getFrameData(int &iterationCount, int &escapeCount,
                      std::vector<double> &magnitudeGrid,
                      std::vector<int> &iterationGrid,
                      std::vector<int> &escapeIterationCounterSums);

    void zoomIn(double factor);
    void zoomOut(double factor);

    void zoomOnPixel(int x, int y);

    void move(double real, double imag);

    void printLocation();

private:
    std::vector<Complex> grid;
    std::vector<int> m_iterationGrid;

    std::vector<double> safe_magnitudeGrid;
    std::vector<int> safe_iterationGrid;

    std::vector<int> escapeIterationCounter;
    std::vector<int> safe_escapeIterationCounterSums;

    int m_escapeCount;
    int safe_escapeCount;
    int m_iterationCount;
    int safe_iterationCount;
    int m_iterationMaximum;
    double m_escapeRadius;
    int m_width, m_height;
    double aspectRatio;
    Complex m_viewCenter;
    double m_viewScale;

    bool isRunning;
    std::mutex calculationMutex;
    bool invalidateCurrentIteration;

    Complex mapToComplex(double x, double y);

    void setValueAt(int x, int y, Complex value);

    void incrementIterationGrid(int x, int y);

    // Iterates over one row of the grid, intended for use in multithreading.
    void rowIterator(WorkQueue *workqueue);

    void iterateGrid();
};

#endif
