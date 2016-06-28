#!/bin/bash
shopt -s extglob
rm -R !(build.sh|clear.sh|Examples|README|run.sh|src)
cd src
rm -R !(main.cpp|BMPFile.h|BMPFile.cpp)
