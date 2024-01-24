#ifndef _MANDELBROTSHADING
#define _MANDELBROTSHADING

#include <tuple>

class Shading
{
public:
    using Colour = std::tuple<int, int, int>;
    using ShadingFunction = Colour (Shading::*)(double);

    Shading();

    Colour shade(double colourFactor);

    void setShadingFunction(int functionNumber);

private:
    ShadingFunction shadingFunction;

    Colour shadeGreyscale(double colourFactor);

    Colour shadeGreyscaleInverse(double colourFactor);

    Colour shadeHsv(double colourFactor);

    Colour hsvToRgb(double hue, double saturation, double value);

};

#endif
