#ifndef PPM_H
#define PPM_H

#include "types.h"
#include <fstream>
#include <iostream>

void ppm(const char* filename, std::vector<rgbf>& data, int width, int height);

#endif
