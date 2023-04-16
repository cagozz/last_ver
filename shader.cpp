#include "shader.h"


rgbf color(Scene& s, Ray& r, int maxDepth)
{
    double t; bool check;
    Object* o = nullptr;
    rgbf pixel;

    t = std::numeric_limits<double>::infinity();

    for(int i = 0; i < s.objects.size(); i++)
    {
        if(s.objects[i]->intersect(r, s))
        {
            if(r.t < t)
            {
                t = r.t;
                auto& elem = s.objects[i];

                if(Sphere* s = dynamic_cast<Sphere*>(elem.get()))
                    o = s;
                else if(Triangle* tr = dynamic_cast<Triangle*>(elem.get()))
                    o = tr;
                else if(Mesh* m = dynamic_cast<Mesh*>(elem.get()))
                    o = m;
            }
        }
    }

    r.t = t;
    r.intersectionPoint = r.origin + r.t * r.direction;

    pixel = s.backgroundColor;

    if(o != nullptr)
    {
        pixel = ambient(s, o->getMaterialIndex());
        Eigen::Vector3d normal= o->normal(s, r);

        for(int i = 0; i < s.pointLights.size(); i++)
        {
            check = false;
            
            Eigen::Vector3d wi = (s.pointLights[i].location - r.intersectionPoint);
            Ray shdwRay{r.intersectionPoint + (wi.normalized()*s.shdwRayEps), wi};

            for(int j = 0; j < s.objects.size(); j++)
            {
                if(s.objects[j]->intersect(shdwRay, s))
                {
                    check = true;
                    break;
                }
            }

            if(check)
                continue;
      
            rgbf diffuseColor = diffuse(s.pointLights[i], r, normal, s.materials[o->getMaterialIndex()-1].diffReflect);
            rgbf specularColor = specular(s.pointLights[i], r, normal, s.materials[o->getMaterialIndex()-1].specReflect,
                                            s.materials[o->getMaterialIndex()-1].phongExp, s.camera);

            pixel.r += diffuseColor.r + specularColor.r;
            pixel.g += diffuseColor.g + specularColor.g;
            pixel.b += diffuseColor.b + specularColor.b;
        }

        if(maxDepth > 0 && mirrorCheck(s.materials[o->getMaterialIndex()-1]))
        {
            Eigen::Vector3d direction = r.direction - 2 * (r.direction.dot(normal)) * normal;

            Ray reflectionRay{r.intersectionPoint+(direction.normalized()*s.shdwRayEps), direction};

            rgbf reflection = color(s, reflectionRay, maxDepth-1);

            pixel.r += s.materials[o->getMaterialIndex()-1].mirrReflect.r * reflection.r;
            pixel.g += s.materials[o->getMaterialIndex()-1].mirrReflect.g * reflection.g;
            pixel.b += s.materials[o->getMaterialIndex()-1].mirrReflect.b * reflection.b;
        }
    }

    return pixel;
}

rgbf ambient(Scene& s, int materialIndex)
{
    rgbf color{0,0,0};

    color.r = float(s.ambientLight.r * s.materials[materialIndex-1].ambReflect.r);
    color.g = float(s.ambientLight.g * s.materials[materialIndex-1].ambReflect.g);
    color.b = float(s.ambientLight.b * s.materials[materialIndex-1].ambReflect.b);

    return color;
}

rgbf diffuse(PointLight& light, Ray& r, Eigen::Vector3d& normal, rgbf& kd)
{
    rgbf color{0,0,0}, intensity;
    double distance = (light.location - r.intersectionPoint).norm();

    intensity.r = light.intensity.r / (distance*distance);
    intensity.g = light.intensity.g / (distance*distance);
    intensity.b = light.intensity.b / (distance*distance);

    Eigen::Vector3d l = (light.location - r.intersectionPoint).normalized();

    double nl = normal.dot(l);
    if(nl <= 0)
        return color;

    color.r = kd.r*intensity.r*nl;
    color.g = kd.g*intensity.g*nl;
    color.b = kd.b*intensity.b*nl;

    return color;
}

rgbf specular(PointLight& light, Ray& r, Eigen::Vector3d& normal, rgbf& ks, double& phong, Camera& c)
{
    rgbf color{0,0,0}, intensity;
    double distance = (light.location - r.intersectionPoint).norm();

    intensity.r = light.intensity.r / (distance*distance);
    intensity.g = light.intensity.g / (distance*distance);
    intensity.b = light.intensity.b / (distance*distance);

    Eigen::Vector3d v = (c.location - r.intersectionPoint).normalized();
    Eigen::Vector3d l = (light.location - r.intersectionPoint).normalized();
    Eigen::Vector3d h = (v+l).normalized();

    double nh = normal.dot(h);

    if(nh <= 0)
        return color;

    color.r = ks.r*intensity.r*pow(nh, phong);
    color.g = ks.g*intensity.g*pow(nh, phong);
    color.b = ks.b*intensity.b*pow(nh, phong);

    return color;
}

int mirrorCheck(const Material& m)
{
    return (m.mirrReflect.r > 0 || m.mirrReflect.g > 0 || m.mirrReflect.b > 0 );
}