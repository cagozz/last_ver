#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>
#include "types.h"
#include <cmath>

struct Ray
{
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
    Eigen::Vector3d intersectionPoint;
    double t;
};

Ray createRay(const Camera& camera, double i, double j);

#endif