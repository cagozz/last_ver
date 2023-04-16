#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>
#include <vector>

typedef struct RGB
{
    int r,g,b;
} rgb;

typedef struct RGBf
{
    double r,g,b;
} rgbf;

struct Camera
{
    Eigen::Vector3d location;
    Eigen::Vector3d gaze;
    Eigen::Vector3d up;
    Eigen::Vector4d plane;
    double distance;
    int height, width;
};

struct Material
{
    int index;
    rgbf ambReflect;
    rgbf diffReflect;
    rgbf specReflect;
    double phongExp;
    rgbf mirrReflect;
};

struct PointLight
{
    int index;
    Eigen::Vector3d location;
    rgbf intensity;
};
/*
struct Sphere
{
    int index;
    int materialIndex;
    int vertexID;
    double radius;
};

struct Triangle
{
    int index;
    int materialIndex;
    int vertices[3];
};

struct Mesh
{
    int index;
    int materialIndex;
    std::vector<Triangle> faces;
};*/

#endif