#include "object.h"

int Sphere::intersect(Ray& r, Scene& scene)
{
    Eigen::Vector3d d = r.direction;
    Eigen::Vector3d oc = r.origin - scene.vertices[vertexID-1];

    double b4ac = pow(d.dot(oc),2)-d.dot(d)*(oc.dot(oc)-pow(radius,2));
    double t;

    if(b4ac >=0)
    {
        t = (-d.dot(oc)-sqrt(b4ac))/(d.dot(d));

        if (t < 0)
            return 0;

        r.t = t;
        r.intersectionPoint = r.origin + r.t * r.direction;

        return 1;
    }
    
    return 0;
}

Eigen::Vector3d Sphere::normal(Scene& scene, Ray& r)
{
    return (r.intersectionPoint-scene.vertices[vertexID-1])/radius;
}

int Triangle::intersect(Ray& r, Scene& scene)
{
    Eigen::Vector3d a(scene.vertices[vertices[0]-1].x(),scene.vertices[vertices[0]-1].y(),scene.vertices[vertices[0]-1].z());
    Eigen::Vector3d b(scene.vertices[vertices[1]-1].x(),scene.vertices[vertices[1]-1].y(),scene.vertices[vertices[1]-1].z());
    Eigen::Vector3d c(scene.vertices[vertices[2]-1].x(),scene.vertices[vertices[2]-1].y(),scene.vertices[vertices[2]-1].z());
    
    Eigen::Matrix3d A;
    A << a.x()-b.x(), a.x()-c.x(), r.direction.x(),
         a.y()-b.y(), a.y()-c.y(), r.direction.y(),
         a.z()-b.z(), a.z()-c.z(), r.direction.z();

    double detA = A.determinant();

    if(detA == 0)
        return 0;

    Eigen::Matrix3d betaMat;
    betaMat <<  a.x()-r.origin.x(), a.x()-c.x(), r.direction.x(),
                a.y()-r.origin.y(), a.y()-c.y(), r.direction.y(),
                a.z()-r.origin.z(), a.z()-c.z(), r.direction.z();

    Eigen::Matrix3d gammaMat;
    gammaMat << a.x()-b.x(), a.x()-r.origin.x(), r.direction.x(),
                a.y()-b.y(), a.y()-r.origin.y(), r.direction.y(),
                a.z()-b.z(), a.z()-r.origin.z(), r.direction.z();

    Eigen::Matrix3d tMat;
    tMat <<     a.x()-b.x(), a.x()-c.x(), a.x()-r.origin.x(),
                a.y()-b.y(), a.y()-c.y(), a.y()-r.origin.y(),
                a.z()-b.z(), a.z()-c.z(), a.z()-r.origin.z();

    double betaVal = betaMat.determinant()/detA;
    double gammaVal = gammaMat.determinant()/detA;
    double tVal = tMat.determinant()/detA;

    if(tVal<0)
        return 0;
    if(gammaVal<0 || gammaVal>1)
        return 0;
    if(betaVal<0 || betaVal>(1-gammaVal))
        return 0;

    r.t = tVal;
    r.intersectionPoint = r.origin + tVal * r.direction;

    return 1;
}

Eigen::Vector3d Triangle::normal(Scene& scene, Ray& r)
{
    return ((scene.vertices[vertices[1]-1] - scene.vertices[vertices[0]-1]).cross(scene.vertices[vertices[2]-1] - scene.vertices[vertices[0]-1])).normalized();
}

int Mesh::intersect(Ray& r, Scene& scene)
{
    double t = std::numeric_limits<double>::infinity();
    bool flag = false;
    int check = 0;
    Triangle face;

    for(int i = 0; i < faces.size(); i++)
    {
        check = faces[i].intersect(r, scene);
        if(check)
        {
            if(r.t < t)
            {
                t = r.t;
                r.intersectionPoint = r.origin + t*r.direction;
                face = faces[i];
            }
            
            flag = true;
            r.t = t;            
        }
    }

    if(flag)
    {
        normalVec = face.normal(scene,r);
        return 1;
    }
        
    return 0;
}