#ifndef SHADER_H
#define SHADER_H

#include "scene.h"
#include "ray.h"
#include <iostream>

rgbf color(Scene& s, Ray& r, int maxDepth);

rgbf ambient(Scene& s, int materialIndex);
rgbf diffuse(PointLight& light, Ray& r, Eigen::Vector3d& normal, rgbf& kd);
rgbf specular(PointLight& light, Ray& r, Eigen::Vector3d& normal, rgbf& ks, double& phong, Camera& c);

int mirrorCheck(const Material& m);

#endif