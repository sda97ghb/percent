#include <cstdlib>
#include <iostream>

#include "BMPFile.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Wrong parameters\n\tUsage: " << argv[0] <<
                     " <image name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto filename = argv[1];

    BMPFile file;
    std::clog << "Open file: " << filename << std::endl;
    file.readBMP(filename);

    std::cout << "Image is " << file.width() << "px width and " <<
                 file.height() << "px height. There are " <<
                 (file.width() * file.height()) << " pixels." << std::endl;

    int blackPixelsN = 0;
    int whitePixelsN = 0;

    for (int i = 0; i < file.height(); ++ i)
        for (int j = 0; j < file.width(); ++ j) {
            if (file.pixelBin(i, j))
                ++ whitePixelsN;
            else
                ++ blackPixelsN;
        }
    std::cout << "There are " <<
                 whitePixelsN << " white pixels and " <<
                 blackPixelsN << " black pixels." << std::endl;
    float percent = 100.0f * static_cast<float>(whitePixelsN) /
            static_cast<float>(blackPixelsN + whitePixelsN);
    std::cout << "The object is " << percent << "% from the image." << std::endl;
    return 0;
}

