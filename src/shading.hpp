#ifndef _MANDELBROTSHADING
#define _MANDELBROTSHADING

#include <vector>
#include <tuple>

class Shading
{
public:
    using Colour = std::tuple<int, int, int>;
    using HsvColour = std::tuple<double, double, double>;
    using ShadingFunction = Colour (Shading::*)(double, double);

    Shading();

    Colour shade(double colourFactor, double velocityFactor);

    void setShadingFunction(int functionNumber);

private:
    ShadingFunction shadingFunction;

    Colour shadeGreyscale(double histogramFactor, double velocityFactor);

    Colour shadeGreyscaleInverse(double histogramFactor, double velocityFactor);

    Colour shadeHsv(double histogramFactor, double velocityFactor);

    Colour shadeMidnightCherry(double histogramFactor, double velocityFactor);

    Colour hsvToRgb(double hue, double saturation, double value);
    Colour hsvToRgb(HsvColour hsvColour);

    Colour colourRamp(std::vector<std::pair<double, HsvColour>> hsvPath, double factor);

    double lerp(double min, double max, double normalizedFactor);
    HsvColour lerp(HsvColour minColour, HsvColour maxColour, double normalizedFactor);

};

#endif
