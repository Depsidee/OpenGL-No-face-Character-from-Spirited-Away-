#pragma once
#include "Point.h"
class Cylinder
{
public:
	Cylinder();
	~Cylinder();
	static void DrawCylinder(Point Base_center1, float Base_rad1, float Base_rad2, float height, int texture);
};

