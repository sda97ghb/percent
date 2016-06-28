#ifndef BMPFILE_H
#define BMPFILE_H

#include <fstream>
#include <iostream>

typedef struct {
      unsigned short bfType;          // 0x4d42 | 0x4349 | 0x5450
      unsigned int   bfSize;          // размер файла
      unsigned int   bfReserved;      // 0
      unsigned int   bfOffBits;       // смещение до поля данных,
                                      // обычно 54 = 16 + biSize
      unsigned int   biSize;          // размер струкуры в байтах:
                                      // 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER)
                                      // или 124(BITMAPV5HEADER)
      unsigned int   biWidth;         // ширина в точках
      unsigned int   biHeight;        // высота в точках
      unsigned short biPlanes;        // всегда должно быть 1
      unsigned short biBitCount;      // 0 | 1 | 4 | 8 | 16 | 24 | 32
      unsigned int   biCompression;   // BI_RGB | BI_RLE8 | BI_RLE4 |
                                      // BI_BITFIELDS | BI_JPEG | BI_PNG
                                      // реально используется лишь BI_RGB
      unsigned int   biSizeImage;     // Количество байт в поле данных
                                      // Обычно устанавливается в 0
      unsigned int   biXPelsPerMeter; // горизонтальное разрешение, точек на дюйм
      unsigned int   biYPelsPerMeter; // вертикальное разрешение, точек на дюйм
      unsigned int   biClrUsed;       // Количество используемых цветов
                                      // (если есть таблица цветов)
      unsigned int   biClrImportant;  // Количество существенных цветов.
                                      // Можно считать, просто 0
} BMPHeader;

class BMPFile
{
private:
    BMPHeader _header;
    unsigned char *_pixels;

public:    
    BMPFile();
    ~BMPFile();

    void readBMPHeader(BMPHeader &header, std::istream &ifile);
    void readBMP(const char *path);

    unsigned width() const;
    unsigned height() const;

    bool pixelBin(unsigned i, unsigned j) const;
};

#endif // BMPFILE_H
