#pragma once
class Point
{
public:
	Point(float x, float y, float z);
	Point(float x, float y, float z, float s, float t);
	~Point(void);
	float x, y, z;
	float s, t;
};

