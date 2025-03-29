#ifndef _MANDELBROTSHADING
#define _MANDELBROTSHADING

#include <tuple>
#include <vector>

// Class for handling shading, in this case meaning:
// Converts a scalar into RGB.
// Does a bit of light functional programming so the shading function can easily
// be switched between some options.
class Shading {
public:
    using Colour = std::tuple<int, int, int>;
    using HsvColour = std::tuple<double, double, double>;
    using ShadingFunction = Colour (Shading::*)(double, double);

    Shading();

    Colour shade(double colourFactor, double timeCounter);

    void setShadingFunction(int functionNumber);

private:
    ShadingFunction shadingFunction;

    Colour shadeGreyscale(double histogramFactor, double timeCounter);

    Colour shadeGreyscaleInverse(double histogramFactor, double timeCounter);

    Colour shadeHsv(double histogramFactor, double timeCounter);

    std::vector<std::pair<double, HsvColour>> midnightCherryPath;
    Colour shadeMidnightCherry(double histogramFactor, double timeCounter);

    Colour hsvToRgb(double hue, double saturation, double value);
    Colour hsvToRgb(HsvColour hsvColour);

    Colour colourRamp(const std::vector<std::pair<double, HsvColour>> &hsvPath,
                      double factor);

    double lerp(double min, double max, double normalizedFactor);
    HsvColour lerp(HsvColour minColour, HsvColour maxColour,
                   double normalizedFactor);
};

#endif
