#pragma once
#include "Mesh.h"

class Quad : public Mesh
{
public:

	Quad(); // defaults to dimensions (1, 1)
	Quad(float width, float height);
};
