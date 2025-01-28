
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include "Point.h"
#include "Cylinder.h"
#include "Texture.h"
#include <fstream>
// skybox
int SKYFRONT, SKYBACK, SKYLEFT, SKYRIGHT, SKYUP, SKYDOWN;

HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Cntext
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

//Camera
double movX, movY, movZ,x=0,z=0;
double lX, lY;
void Camera()
{
	gluLookAt(movX, movY, movZ, lX, lY, -5, 0, 1, 0);
	if (keys['Z'])
		x += 1;
	if (keys['X'])
		x -= 1;
	if (keys['C'])
		z += 1;
	if (keys['V'])
		z -= 1;
	if (keys['Z'])
		lX += 0.001;
	if (keys['X'])
		lX -= 0.001;
	if (keys['C'])
		lY += 0.001;
	if (keys['V'])
		lY -= 0.001;
}

//SkyBox
void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SKYFRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SKYBACK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SKYLEFT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SKYRIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SKYUP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SKYDOWN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glColor3f(1, 1, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
//---------------------------ball (sphere with texture)---------------------------
void drawBall(float rad, int texture, Point center)
{
	glTranslated(center.x, center.y, center.z);
	float lastcenter = rad*sin(3.14 / 2);
	float lastr = rad*cos(3.14 / 2);
	for (float vertical = 3.14 * 2.0; vertical >= 0; vertical -= 0.1)
	{

		float centeri = rad*sin(vertical);
		float ri = rad*cos(vertical);
		int col = 0;
		for (float horizin = 0; horizin <= 2 * 3.14 + 0.1; horizin += 0.1)
		{
			Point a = Point(ri*cos(horizin), centeri, ri*sin(horizin));
			Point b = Point(ri*cos(horizin + 0.1), centeri, ri*sin(horizin + 0.1));
			Point c = Point(lastr*cos(horizin), lastcenter, lastr*sin(horizin));
			Point d = Point(lastr*cos(horizin + 0.1), lastcenter, lastr*sin(horizin + 0.1));
			if (texture != -1)
			{
				glBindTexture(GL_TEXTURE_2D, texture);
				glBegin(GL_QUADS);
				glTexCoord2d(1 - (horizin) / (2 * 3.14), 0.5 + sin(vertical - 0.1));
				glVertex3f(a.x, a.y, a.z);
				glTexCoord2d(1 - (horizin + 0.1) / (2 * 3.14), 0.5 + sin(vertical - 0.1));
				glVertex3f(b.x, b.y, b.z);
				glTexCoord2d(1 - (horizin + 0.1) / (2 * 3.14), 0.5 + sin(vertical));
				glVertex3f(d.x, d.y, d.z);
				glTexCoord2d(1 - (horizin) / (2 * 3.14), 0.5 + sin(vertical));
				glVertex3f(c.x, c.y, c.z);
				glEnd();
			}
			else
			{
				glBegin(GL_QUADS);
				glVertex3f(a.x, a.y, a.z);
				glVertex3f(b.x, b.y, b.z);
				glVertex3f(d.x, d.y, d.z);
				glVertex3f(c.x, c.y, c.z);
				glEnd();
			}
		}
		lastr = ri;
		lastcenter = centeri;
	}

	glTranslated(-center.x, -center.y, -center.z);

}

//---------------------------half ball (half sphere with texture)---------------------------
void drawHalfBall(float rad, int texture, Point center)
{
	glTranslated(center.x, center.y, center.z);
	float lastcenter = rad*sin(3.14 / 2);
	float lastr = rad*cos(3.14 / 2);
	for (float vertical = 3.14 / 2.0; vertical >= 0; vertical -= 0.1)
	{

		float centeri = rad*sin(vertical);
		float ri = rad*cos(vertical);
		int col = 0;
		for (float horizin = 0; horizin <= 2 * 3.14 + 0.1; horizin += 0.1)
		{
			Point a = Point(ri*cos(horizin), centeri, ri*sin(horizin));
			Point b = Point(ri*cos(horizin + 0.1), centeri, ri*sin(horizin + 0.1));
			Point c = Point(lastr*cos(horizin), lastcenter, lastr*sin(horizin));
			Point d = Point(lastr*cos(horizin + 0.1), lastcenter, lastr*sin(horizin + 0.1));
			if (texture != -1)
			{
				glBindTexture(GL_TEXTURE_2D, texture);
				glBegin(GL_QUADS);
				glTexCoord2d(1 - (horizin) / (2 * 3.14), 0.5 + sin(vertical - 0.1) / 2.0);
				glVertex3f(a.x, a.y, a.z);
				glTexCoord2d(1 - (horizin + 0.1) / (2 * 3.14), 0.5 + sin(vertical - 0.1) / 2.0);
				glVertex3f(b.x, b.y, b.z);
				glTexCoord2d(1 - (horizin + 0.1) / (2 * 3.14), 0.5 + sin(vertical) / 2.0);
				glVertex3f(d.x, d.y, d.z);
				glTexCoord2d(1 - (horizin) / (2 * 3.14), 0.5 + sin(vertical) / 2.0);
				glVertex3f(c.x, c.y, c.z);
				glEnd();
			}
			else
			{
				glBegin(GL_QUADS);
				glVertex3f(a.x, a.y, a.z);
				glVertex3f(b.x, b.y, b.z);
				glVertex3f(d.x, d.y, d.z);
				glVertex3f(c.x, c.y, c.z);
				glEnd();
			}
		}
		lastr = ri;
		lastcenter = centeri;
	}
	glTranslated(-center.x, -center.y, -center.z);

}

void drawCube(float sX, float sY, float sZ, int texture, float num){


	float tX = sX*num;
	float tZ = sZ*num;
	float tY = sY*num;

	glPushMatrix();
	//back
	glBindTexture(GL_TEXTURE_2D, texture);
	//glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
	glTexCoord2f(tX, 0);	glVertex3f(sX, 0, 0);
	glTexCoord2f(tX, tY);	glVertex3f(sX, sY, 0);
	glTexCoord2f(0, tY);	glVertex3f(0, sY, 0);
	glEnd();


	//right
	//glColor3f(0, 1, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslated(sX, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
	glTexCoord2f(tZ, 0);	glVertex3f(sZ, 0, 0);
	glTexCoord2f(tZ, tY);	glVertex3f(sZ, sY, 0);
	glTexCoord2f(0, tY);	glVertex3f(0, sY, 0);
	glEnd();

	//front
	//glColor3f(1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslated(sZ, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
	glTexCoord2f(tX, 0);	glVertex3f(sX, 0, 0);
	glTexCoord2f(tX, tY);	glVertex3f(sX, sY, 0);
	glTexCoord2f(0, tY);	glVertex3f(0, sY, 0);
	glEnd();

	//left
	//glColor3f(1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslated(sX, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
	glTexCoord2f(tZ, 0);	glVertex3f(sZ, 0, 0);
	glTexCoord2f(tZ, tY);	glVertex3f(sZ, sY, 0);
	glTexCoord2f(0, tY);	glVertex3f(0, sY, 0);
	glEnd();
	glPopMatrix(); //reset the origin

	//top
	//glColor3f(0, 0, 1);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslated(0, sY, 0);
	glRotated(-90, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
	glTexCoord2f(tX, 0);	glVertex3f(sX, 0, 0);
	glTexCoord2f(tX, tZ);	glVertex3f(sX, sZ, 0);
	glTexCoord2f(0, tZ);	glVertex3f(0, sZ, 0);
	glEnd();
	glPopMatrix(); //reset the origin

	//bottom
	//glColor3f(1, 1, 1);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glRotated(-90, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
	glTexCoord2f(tX, 0);	glVertex3f(sX, 0, 0);
	glTexCoord2f(tX, tZ);	glVertex3f(sX, sZ, 0);
	glTexCoord2f(0, tZ);	glVertex3f(0, sZ, 0);
	glEnd();

	glPopMatrix(); //reset the origin after drawing
	glPopMatrix(); //reset the origin after drawing




}





int black, mask;
int InitGL(GLvoid)
										// All Setup For OpenGL Goes Here
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glEnable(GL_TEXTURE_2D);
	// skybox
	SKYFRONT = LoadTexture("front.bmp", 255);
	SKYBACK = LoadTexture("back.bmp", 255);
	SKYLEFT = LoadTexture("left.bmp", 255);
	SKYRIGHT = LoadTexture("right.bmp", 255);
	SKYUP = LoadTexture("up.bmp", 255);
	SKYDOWN = LoadTexture("down.bmp", 255);
	//Noface
	black = LoadTexture("black.bmp", 255);
	mask = LoadTexture("mask.bmp", 255);
	glDisable(GL_TEXTURE_2D);

	return TRUE;										// Initialization Went OK
}

double X = 0, Y = 0;
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    glTranslated(0, 0, -8);
	glEnable(GL_DEPTH_TEST);

	Point point = Point(0, 0, 0);

	Camera();
	Draw_Skybox(0, 0, 0, 100, 100, 100);

	//cylinder
	glPushMatrix();
	glTranslated(X, 0, Y);
	Cylinder cylinder =Cylinder();
	cylinder.DrawCylinder(point, 1 , 1 , 10, black);
	drawHalfBall(1, black, Point(0,-0.07,0));
	glPushMatrix();
	glTranslated(-0.5, -1, 1.1);
	drawCube(1,1,0.1,mask,1);
	glPopMatrix();
	glPopMatrix();

	if (keys[VK_LEFT])
		X += 0.01;
	if (keys[VK_RIGHT])
		X -= 0.01;
	if (keys[VK_UP])
		Y -= 0.05;
	if (keys[VK_DOWN])
		Y += 0.05;
	
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "GL template", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
													if (!HIWORD(wParam))					// Check Minimization State
													{
														active = TRUE;						// Program Is Active
													}
													else
													{
														active = FALSE;						// Program Is No Longer Active
													}

													return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
													switch (wParam)							// Check System Calls
													{
													case SC_SCREENSAVE:					// Screensaver Trying To Start?
													case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
														return 0;							// Prevent From Happening
													}
													break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
													PostQuitMessage(0);						// Send A Quit Message
													return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
													keys[wParam] = TRUE;					// If So, Mark It As TRUE
													return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
													keys[wParam] = FALSE;					// If So, Mark It As FALSE
													return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
													ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
													return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Example", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("OpenGL template", 640, 480, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

