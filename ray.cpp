#include "ray.h"

Ray createRay(const Camera& camera, double i, double j)
{
    Eigen::Vector3d v = camera.up.normalized();
    Eigen::Vector3d u = (camera.gaze.normalized().cross(v)).normalized();
    double left = camera.plane.x();
    double right = camera.plane.y();
    double bottom = camera.plane.z();
    double top = camera.plane.w();

    Eigen::Vector3d m = camera.location + camera.gaze.normalized() * camera.distance;
    Eigen::Vector3d q = m + camera.plane.x()*u + camera.plane.w()*v;

    double su = (right - left) * (i + .5) / camera.width;
    double sv = (top - bottom) * (j + .5) / camera.height;

    Eigen::Vector3d s = q + su*u - sv*v; 

    return {camera.location , (s - camera.location), {0,0,0}, 0};
}
