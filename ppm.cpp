#include "ppm.h"

void ppm(const char* fileName, std::vector<rgbf>& data, int width, int height)
{
    std::fstream output;
    output.open(fileName, std::ios::out);

    if (!output) 
        std::cout << "File cannot be opened" << std::endl;
    else
    {
        output << "P3\n" << width << " "<< height << "\n" << "255\n";

        long count = 0;
        for (long j = 0; j < height; ++j)
        {
            for (long i = 0; i < width; ++i)
            {
                    output << (int)data[count].r << " " <<  (int)data[count].g << " " << (int)data[count].b << "\n";
                    count++;
            }

            output << "\n";
        }
    }

    output.close();
}