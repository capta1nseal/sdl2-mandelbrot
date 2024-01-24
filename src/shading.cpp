#include "shading.hpp"

#include <tuple>
#include <cmath>

Shading::Shading()
{
    shadingFunction = &Shading::shadeGreyscale;
}

Shading::Colour Shading::shade(double colourFactor)
{
    return (this->*shadingFunction)(colourFactor);
}

void Shading::setShadingFunction(int functionNumber)
{
    switch (functionNumber)
    {
    case 0:
        shadingFunction = &Shading::shadeGreyscale;
        break;
    case 1:
        shadingFunction = &Shading::shadeGreyscaleInverse;
        break;
    case 2:
        shadingFunction = &Shading::shadeHsv;
        break;
    default:
        setShadingFunction(0);
        break;
    }
}

Shading::Colour Shading::shadeGreyscale(double colourFactor)
{
    int alpha = colourFactor * 255;

    return {alpha, alpha, alpha};
}

Shading::Colour Shading::shadeGreyscaleInverse(double colourFactor)
{
    return shadeGreyscale(1.0 - colourFactor);
}

Shading::Colour Shading::shadeHsv(double colourFactor)
{
    colourFactor = 1.0 - colourFactor;
    return hsvToRgb(colourFactor * 360, 0.5 + colourFactor / 2, 0.5 + colourFactor / 2);
}

Shading::Colour Shading::hsvToRgb(double hue, double saturation, double value)
{
    hue = fmodf64(hue, 360.0) / 60.0;
    long i = static_cast<int>(hue);
    double ff = hue - i;
    double p = value * (1.0 - saturation);
    double q = value * (1.0 - (saturation * ff));
    double t = value * (1.0 - (saturation * (1.0 - ff)));

    int r;
    int g;
    int b;

    switch (i)
    {
    case 0:
        r = 255 * value;
        g = 255 * t;
        b = 255 * p;
        break;
    case 1:
        r = 255 * q;
        g = 255 * value;
        b = 255 * p;
        break;
    case 2:
        r = 255 * p;
        g = 255 * value;
        b = 255 * t;
        break;
    case 3:
        r = 255 * p;
        g = 255 * q;
        b = 255 * value;
        break;
    case 4:
        r = 255 * t;
        g = 255 * p;
        b = 255 * value;
        break;
    case 5:
    default:
        r = 255 * value;
        g = 255 * p;
        b = 255 * q;
        break;
    }

    return {r, g, b};
}