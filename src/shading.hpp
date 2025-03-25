#ifndef _MANDELBROTSHADING
#define _MANDELBROTSHADING

#include <tuple>
#include <vector>

class Shading {
  public:
    using Colour = std::tuple<int, int, int>;
    using HsvColour = std::tuple<double, double, double>;
    using ShadingFunction = Colour (Shading::*)(double);

    Shading();

    Colour shade(double colourFactor);

    void setShadingFunction(int functionNumber);

  private:
    ShadingFunction shadingFunction;

    Colour shadeGreyscale(double histogramFactor);

    Colour shadeGreyscaleInverse(double histogramFactor);

    Colour shadeHsv(double histogramFactor);

    std::vector<std::pair<double, HsvColour>> midnightCherryPath;
    Colour shadeMidnightCherry(double histogramFactor);

    Colour hsvToRgb(double hue, double saturation, double value);
    Colour hsvToRgb(HsvColour hsvColour);

    Colour colourRamp(const std::vector<std::pair<double, HsvColour>> &hsvPath,
                      double factor);

    double lerp(double min, double max, double normalizedFactor);
    HsvColour lerp(HsvColour minColour, HsvColour maxColour,
                   double normalizedFactor);
};

#endif
