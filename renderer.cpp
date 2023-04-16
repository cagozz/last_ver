#include "renderer.h"

void render(Scene& s)
{
    int width = s.camera.width, height = s.camera.height;
    std::vector<rgbf> pixels;
    pixels.reserve(width * height);
    rgbf pixel;

    for(int h = 0; h < height; h++)
    {
        for(int w = 0; w < width; w++)
        {
            Ray r = createRay(s.camera, w, h);
            pixel = color(s, r, s.maxRecDepth, h, w);
            pixels.push_back(pixel);
        }
    }

    ppm("output.ppm", pixels, width, height);
}