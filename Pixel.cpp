#include "Pixel.h"

Pixel::Pixel() :
    _r(0),
    _g(0),
    _b(0)
{
}

Pixel::Pixel(int r, int g, int b) :
    _r(r),
    _g(g),
    _b(b)
{
}

void
Pixel::set(int r, int g, int b)
{
    _r = r;
    _g = g;
    _b = b;
}

int
Pixel::r() const
{
    return _r;
}

int
Pixel::g() const
{
    return _g;
}

int
Pixel::b() const
{
    return _b;
}

int
Pixel::value() const
{
    auto max = [] (int a, int b) {
        return a >= b ? a : b;
    };
    auto m = max(_r, max(_g, _b));
    return static_cast<float>(m) / 255.0f * 100.0f;
}
