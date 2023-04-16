#include "parser.h"


void parser(Scene& s, char fileName[50])
{
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            if(line[0] == '#')
            {
                if (line.substr(0, 17) == "#BackgroundColor") 
                {
                    std::getline(file, line);
                    std::istringstream(line) >> s.backgroundColor.r >> s.backgroundColor.g >> s.backgroundColor.b;
                    continue;
                }
                else if(line.substr(0, 19) == "#MaxRecursionDepth")
                {
                    std::getline(file, line);
                    std::istringstream(line) >> s.maxRecDepth;
                    continue;
                }
                else if(line.substr(0, 18) == "#ShadowRayEpsilon")
                {
                    std::getline(file, line);
                    std::istringstream(line) >> s.shdwRayEps;
                    continue;
                }
                else if(line.substr(0, 8) == "#Camera")
                {
                    setCameraSettings(s, file);
                    continue;
                }
                else if(line.substr(0, 15) == "#AmbientLight")
                {
                    std::getline(file, line);
                    std::istringstream(line) >> s.ambientLight.r >> s.ambientLight.g >> s.ambientLight.b;
                    continue;
                }
                else if(line.substr(0, 10) == "#Material")
                {
                    addMaterial(s, file);
                    continue;
                }
                else if(line.substr(0, 12) == "#PointLight")
                {
                    addPointLight(s, file);
                    continue;
                }
                else if(line.substr(0, 8) == "#Sphere")
                {
                    addSphere(s, file);
                    continue;
                }
                else if(line.substr(0, 12) == "#Triangle")
                {
                    addTriangle(s, file);
                    continue;
                }
                else if(line.substr(0, 6) == "#Mesh")
                {
                    addMesh(s, file);
                    continue;
                }
                else if(line.substr(0, 13) == "#VertexList")
                {
                    addVertices(s, file);
                    continue;
                }
            }
        }
    }
    else
    {
        std::cout << "File cannot be opened" << std::endl;
		exit(0);
    }

}

void setCameraSettings(Scene& s, std::fstream& f)
{
    std::string line;
    std::getline(f, line);
    std::istringstream(line) >> s.camera.location.x() >> s.camera.location.y() >> s.camera.location.z();
    std::getline(f, line);
    std::istringstream(line) >> s.camera.gaze.x() >> s.camera.gaze.y() >> s.camera.gaze.z();
    std::getline(f, line);
    std::istringstream(line) >> s.camera.up.x() >> s.camera.up.y() >> s.camera.up.z();
    std::getline(f, line);
    std::istringstream(line) >> s.camera.plane.x() >> s.camera.plane.y() >> s.camera.plane.z() >> s.camera.plane.w();
    std::getline(f, line);
    std::istringstream(line) >> s.camera.distance;
    std::getline(f, line);
    std::istringstream(line) >> s.camera.width >> s.camera.height;
}

void addMaterial(Scene& s, std::fstream& f)
{
    std::string line;
    Material m;
    std::getline(f, line);
    std::istringstream(line) >> m.index;
    std::getline(f, line);
    std::istringstream(line) >> m.ambReflect.r >> m.ambReflect.g >> m.ambReflect.b;
    std::getline(f, line);
    std::istringstream(line) >> m.diffReflect.r >> m.diffReflect.g >> m.diffReflect.b;
    std::getline(f, line);
    std::istringstream(line) >> m.specReflect.r >> m.specReflect.g >> m.specReflect.b;
    std::getline(f, line);
    std::istringstream(line) >> m.phongExp;
    std::getline(f, line);
    std::istringstream(line) >> m.mirrReflect.r >> m.mirrReflect.g >> m.mirrReflect.b;

    s.materials.push_back(m);
}

void addPointLight(Scene& s, std::fstream& f)
{
    std::string line;
    PointLight pl;

    std::getline(f, line);
    std::istringstream(line) >> pl.index;
    std::getline(f, line);
    std::istringstream(line) >> pl.location.x() >> pl.location.y() >> pl.location.z();
    std::getline(f, line);
    std::istringstream(line) >> pl.intensity.r >> pl.intensity.g >> pl.intensity.b;

    s.pointLights.push_back(pl);
}

void addSphere(Scene& s, std::fstream& f)
{
    std::string line;

    int index, materialIndex, vertexID;
    double radius;

    std::getline(f, line);
    std::istringstream(line) >> index;
    std::getline(f, line);
    std::istringstream(line) >> materialIndex;
    std::getline(f, line);
    std::istringstream(line) >> vertexID;
    std::getline(f, line);
    std::istringstream(line) >> radius;

    //Sphere sp(index, materialIndex, vertexID, radius);
    std::unique_ptr<Sphere> sp(new Sphere(index, materialIndex, vertexID, radius));
    s.objects.push_back(std::move(sp));
}

void addTriangle(Scene& s, std::fstream& f)
{
    std::string line;
    int index, materialIndex, vertices[3];

    std::getline(f, line);
    std::istringstream(line) >> index;
    std::getline(f, line);
    std::istringstream(line) >> materialIndex;
    std::getline(f, line);
    std::istringstream(line) >> vertices[0] >> vertices[1] >> vertices[2];

    std::unique_ptr<Triangle> t(new Triangle(index, materialIndex, vertices));
    s.objects.push_back(std::move(t));
}

void addMesh(Scene& s, std::fstream& f)
{
    std::string line;
    int index, materialIndex, vertices[3];


    std::getline(f, line);
    std::istringstream(line) >> index;
    std::getline(f, line);
    std::istringstream(line) >> materialIndex;

    std::unique_ptr<Mesh> m(new Mesh(index, materialIndex));

    do
    {
        std::getline(f, line);
        if(line.size() == 0 || f.eof())
            break;
        std::istringstream(line) >> vertices[0] >> vertices[1] >> vertices[2];
        m->addFace(Triangle(0, materialIndex, vertices));
    } while (line.size() != 0 || !f.eof());

    s.objects.push_back(std::move(m));
}

void addVertices(Scene& s, std::fstream& f)
{
    std::string line;
    do
    {
        Eigen::Vector3d v;
        std::getline(f, line);
        if(line.size() == 0 || f.eof())
            break;
        std::istringstream(line) >> v.x() >> v.y() >> v.z();
        s.vertices.push_back(v);
    } while (line.size() != 0 || !f.eof());
}