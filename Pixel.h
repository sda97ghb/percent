#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
    Pixel();
    Pixel(int r, int g, int b);

    void set(int r, int g, int b);

    int r() const;
    int g() const;
    int b() const;

    int value() const;

private:
    int _r;
    int _g;
    int _b;
};

#endif // PIXEL_H
