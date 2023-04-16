#ifndef SCENE_H
#define SCENE_H

class Object;
#include "types.h"
#include "object.h"
#include <memory>


struct Scene
{
        rgbf backgroundColor;
        rgbf ambientLight;
        int maxRecDepth;
        double shdwRayEps;
        Camera camera;
        std::vector<Material> materials;
        std::vector<PointLight> pointLights;
        std::vector<std::unique_ptr<Object>> objects;
        std::vector<Eigen::Vector3d> vertices;
};

#endif