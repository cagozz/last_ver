#ifndef OBJECT_H
#define OBJECT_H

struct Scene;
struct Ray;
#include "scene.h"
#include "ray.h"

class Object
{
    public:
        virtual ~Object() = default;
        virtual int intersect(Ray& r, Scene& scene) { return 0; };
        virtual int getMaterialIndex() { return -1; };
        virtual Eigen::Vector3d normal(Scene& scene, Ray& r) { return {0,0,0}; };
};

class Triangle : public Object
{
    private:
        int index;
        int materialIndex;
        int vertices[3];
    public:
        Triangle() {};
        Triangle(int ind, int matInd, int ver[]) : index(ind), materialIndex(matInd), vertices{ver[0], ver[1], ver[2]} {};

        int intersect(Ray& r, Scene& scene);
        Eigen::Vector3d normal(Scene& scene, Ray& r);
        int getMaterialIndex() override { return materialIndex; };
};

class Sphere : public Object
{
    private:
        int index;
        int materialIndex;
        int vertexID;
        double radius;
    public:
        Sphere(int ind, int matInd, int id, double r) : index(ind), materialIndex(matInd), vertexID(id), radius(r) {};
        
        int intersect(Ray& r, Scene& scene);
        Eigen::Vector3d normal(Scene& scene, Ray& r);
        int getMaterialIndex() { return materialIndex; }
};

class Mesh : public Object
{
    private:
        int index;
        int materialIndex;
        std::vector<Triangle> faces;
        Eigen::Vector3d normalVec;
    public:
        Mesh() {}
        Mesh(int ind, int matInd) : index(ind), materialIndex(matInd) {}
        
        int intersect(Ray& r, Scene& scene);
        Eigen::Vector3d normal(Scene& scene, Ray& r) { return normalVec; };
        int getMaterialIndex() { return materialIndex; }
        inline void addFace(Triangle t) { faces.push_back(t); }
};

#endif