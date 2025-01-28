#define _USE_MATH_DEFINES
#include "Cylinder.h"
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include <math.h>
#include "Texture.h"


Cylinder::Cylinder()
{
}


Cylinder::~Cylinder()
{
}
void Cylinder::DrawCylinder(Point Base_center1, float Base_rad1, float Base_rad2, float height, int texture)
{
	glPushMatrix();
	glTranslated(Base_center1.x, Base_center1.y, Base_center1.z);
	glRotated(90,1, 0, 0);
	Base_center1 = Point(0, 0, 0);
	Point Base_center2 = Point(0, 0, height);
	if (texture != -1)
		glColor3f(1, 1, 1);
	float angle = 0, x1 = Base_rad1, y1 = 0, x2 = Base_rad2, y2 = 0;
	for (angle = 0; angle <= 2 * 3.14 + 0.05; angle += 0.05)
	{
		Point a = Point(x1, y1, Base_center1.z);
		Point b = Point(x2, y2, Base_center2.z);
		x1 = Base_rad1*cos(angle);
		y1 = Base_rad1*sin(angle);
		x2 = Base_rad2*cos(angle);
		y2 = Base_rad2*sin(angle);
		Point c = Point(x2, y2, Base_center2.z);
		Point d = Point(x1, y1, Base_center1.z);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);

		glTexCoord2f(angle / 2 * 3.14, a.z / height);
		glVertex3f(a.x, a.y, a.z);

		glTexCoord2f(angle / 2 * 3.14, b.z / height);
		glVertex3f(b.x, b.y, b.z);

		glTexCoord2f((angle + 0.1) / 2 * 3.14, c.z / height);
		glVertex3f(c.x, c.y, c.z);

		glTexCoord2f((angle + 0.1) / 2 * 3.14, d.z / height);
		glVertex3f(d.x, d.y, d.z);

		glEnd();
	}
	glTranslated(-Base_center1.x, -Base_center1.y, -Base_center1.z);
	glPopMatrix();

}


