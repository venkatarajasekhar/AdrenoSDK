
#ifdef WIN32
#include <GLEW/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

void getWindowDimension(int& width, int& height)
{
	struct { GLint x, y, width, height; } viewport;
	glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);

	width = viewport.width;
	height = viewport.height;
}