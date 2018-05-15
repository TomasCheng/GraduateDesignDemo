#include "QuardMesh.h"

// --------------------------------------------------------------------------------------------
Quad::Quad()
{
	Name = "quadMesh";

	Positions =
	{
		{ -1.0f,  1.0f, 0.0f, },
	{ -1.0f, -1.0f, 0.0f, },
	{ 1.0f,  1.0f, 0.0f, },
	{ 1.0f, -1.0f, 0.0f, },
	};
	UV = {
		{ 0.0f, 1.0f, },
	{ 0.0f, 0.0f, },
	{ 1.0f, 1.0f, },
	{ 1.0f, 0.0f, },
	};
	Topology = TRIANGLE_STRIP;

	Finalize();
}
// --------------------------------------------------------------------------------------------
Quad::Quad(float width, float height)
{
	Name = "quadMesh";

	Positions =
	{
		{ -width,  height, 0.0f, },
	{ -width, -height, 0.0f, },
	{ width,  height, 0.0f, },
	{ width, -height, 0.0f, },
	};
	UV = {
		{ 0.0f, 1.0f, },
	{ 0.0f, 0.0f, },
	{ 1.0f, 1.0f, },
	{ 1.0f, 0.0f, },
	};
	Topology = TRIANGLE_STRIP;

	Finalize();
}