#include "BMPFile.h"

BMPFile::BMPFile()
{
}

BMPFile::~BMPFile()
{
    delete [] _pixels;
}

void
BMPFile::readBMPHeader(BMPHeader &header, std::istream &ifile)
{
    char *ch = new char [54];
    char *p = ch;

    ifile.read((char *)ch, 54);

    _header.bfType          = *(unsigned short *)p; p += 2;
    _header.bfSize          = *(unsigned int   *)p; p += 4;
    _header.bfReserved      = *(unsigned int   *)p; p += 4;
    _header.bfOffBits       = *(unsigned int   *)p; p += 4;

    _header.biSize          = *(unsigned int   *)p; p += 4;
    _header.biWidth         = *(unsigned int   *)p; p += 4;
    _header.biHeight        = *(unsigned int   *)p; p += 4;

    _header.biPlanes        = *(unsigned short *)p; p += 2;

    _header.biBitCount      = *(unsigned short *)p; p += 2;
    _header.biCompression   = *(unsigned int   *)p; p += 4;

    _header.biSizeImage     = *(unsigned int   *)p; p += 4;

    _header.biXPelsPerMeter = *(unsigned int   *)p; p += 4;
    _header.biYPelsPerMeter = *(unsigned int   *)p; p += 4;

    _header.biClrUsed       = *(unsigned int   *)p; p += 4;
    _header.biClrImportant  = *(unsigned int   *)p; p += 4;

    delete [] ch;
}

void
BMPFile::readBMP(const char *path)
{
    std::ifstream ifile(path, std::ios::binary);

    if (!ifile.is_open())
        exit(EXIT_FAILURE);

    readBMPHeader(_header, ifile);

    _pixels = new unsigned char [_header.biSizeImage];
    ifile.read((char *)_pixels, _header.biSizeImage);

    ifile.close();
}

unsigned
BMPFile::width() const
{
    return _header.biWidth;
}

unsigned
BMPFile::height() const
{
    return _header.biHeight;
}

bool
BMPFile::pixelBin(unsigned i, unsigned j) const
{
    unsigned bytej = (j + 1) / 8 + ((j + 1) % 8 == 0 ? 0 : 1);
    unsigned bytesWidth = width() / 8 + (width() % 8 == 0 ? 0 : 1);
    bytesWidth += bytesWidth % 4;
    auto pixelPtr = _pixels + (bytesWidth * i + bytej);
    return *pixelPtr & (128 >> (j % 8));
}
