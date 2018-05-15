#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{
public:
	Sphere(unsigned int xSegments = 64, unsigned int ySegments = 64);
};
