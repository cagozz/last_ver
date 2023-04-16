#ifndef PARSER_H
#define PARSER_H

#include "scene.h"
#include "object.h"
#include <fstream>
#include <iostream>
#include <string>

void parser(Scene&, char[50]);
void setCameraSettings(Scene&, std::fstream&);
void addMaterial(Scene&, std::fstream&);
void addPointLight(Scene&, std::fstream&);
void addSphere(Scene&, std::fstream&);
void addTriangle(Scene&, std::fstream&);
void addMesh(Scene&, std::fstream&);
void addVertices(Scene&, std::fstream&);

#endif