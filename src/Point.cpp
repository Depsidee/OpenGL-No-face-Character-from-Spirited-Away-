#include "Point.h"


Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->s = -1;
	this->t = -1;
}
Point::Point(float x, float y, float z, float s, float t){

	this->x = x;
	this->y = y;
	this->z = z;
	this->s = s;
	this->t = t;
}

Point::~Point(void)
{
}