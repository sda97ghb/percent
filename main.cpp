#include <iostream>
#include <fstream>
#include <functional>
#include <Matrices.hpp>
#include <Pixel.h>

Matrix<Pixel> loadBmp(std::string filename);
Matrix<bool> threshold(Matrix<Pixel> matrix, std::function<float(Pixel)> f,
                       float threshold);

int main(int argc, char** argv)
{
    std::string USAGE = std::string("Usage: ") + argv[0] +
            std::string(" [negate] <image name>");
    if (argc < 2 || argc > 3) {
        std::cerr << "Wrong numer of parameters." << std::endl;
        std::cout << USAGE << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string filename;
    bool negate;

    if (argc == 2) {
        filename = argv[1];
        negate = false;
    } else if (std::string(argv[1]) == "negate") {
        filename = argv[2];
        negate = true;
    } else {
        std::cout << argv[1] << " is not valid argument." << std::endl;
        std::cout << USAGE << std::endl;
    }

    Matrix<Pixel> matrix = loadBmp(filename);

    if (matrix.rows() == 0 || matrix.columns() == 0) {
        std::cerr << "Error: image is emptry" << std::endl;
        exit(EXIT_FAILURE);
    }

    float lightest = 0.0f;   //   0%
    float darkest  = 100.0f; // 100%
    for (Pixel i : matrix) {
        if (i.value() > lightest)
            lightest = i.value();
        if (i.value() < darkest)
            darkest = i.value();
    }
    float average = (lightest + darkest) / 2.0f;
    std::cout << "Darkest:  " << darkest  << "%" << std::endl;
    std::cout << "Lightest: " << lightest << "%" << std::endl;
    std::cout << "Average:  " << average  << "%" << std::endl;

    std::cout << "Binarization... ";
    Matrix<bool> binarized = threshold(matrix, [] (Pixel pixel) {
        return pixel.value();
    }, average);
    std::cout << "Done." << std::endl;

    int blackPixelsN = 0;
    int whitePixelsN = 0;

    for (int i = 0; i < binarized.rows(); ++ i)
        for (int j = 0; j < binarized.columns(); ++ j) {
            if (binarized.get(i, j))
                ++ whitePixelsN;
            else
                ++ blackPixelsN;
        }

    if (negate)
        std::swap(blackPixelsN, whitePixelsN);

    std::cout << "There are " <<
                 whitePixelsN << " white pixels and " <<
                 blackPixelsN << " black pixels." << std::endl;
    float percent = 100.0f * static_cast<float>(whitePixelsN) /
            static_cast<float>(blackPixelsN + whitePixelsN);
    std::cout << "The object is " << percent << "% from the image." << std::endl;

    return EXIT_SUCCESS;
}

Matrix<Pixel>
loadBmp(std::string filename)
{
    std::ifstream file(filename, std::ios_base::binary);

    if (!file.is_open()) {
        std::cerr << "Can't open file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    } else
        std::cout << "Open file: " << filename << std::endl;

    const size_t BMP_HEADER_SIZE = 54;
    unsigned char *header = new unsigned char [BMP_HEADER_SIZE];
    file.read(reinterpret_cast<char *>(header), BMP_HEADER_SIZE);
    int width  = *reinterpret_cast<int *>(header + 0x12);
    int height = *reinterpret_cast<int *>(header + 0x16);
    int size   = *reinterpret_cast<int *>(header + 0x22);
    std::cout << "Image is " << width << "px in width and " <<
                 height << "px in height. There are " <<
                 width * height << " pixels." << std::endl;
    delete [] header;

    int STRING_LENGTH = width * 3;
    STRING_LENGTH += ((STRING_LENGTH % 4 == 0) ? 0 : (4 - STRING_LENGTH % 4));
    Matrix<Pixel> matrix(height, width);

    unsigned char *content = new unsigned char [size];
    file.read(reinterpret_cast<char *>(content), size);
    for (int i = 0; i < height; ++ i)
        for (int j = 0; j < width; ++ j) {
            unsigned char *pixel = content + STRING_LENGTH * i + 3 * j;
            matrix.set(i, j, Pixel(pixel[0], pixel[1], pixel[2]));
        }
    delete [] content;

    file.close();
    return matrix;
}

Matrix<bool>
threshold(Matrix<Pixel> matrix, std::function<float(Pixel)> f, float threshold)
{
    Matrix<bool> binarized = Matrix<bool>(matrix.rows(), matrix.columns());
    for (int i = 0; i < binarized.rows(); ++ i)
        for (int j = 0; j < binarized.columns(); ++ j)
            binarized.set(i, j, f(matrix.get(i, j)) >= threshold);
    return binarized;
}
