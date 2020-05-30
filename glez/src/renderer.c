#include "type.h"
#include "renderer.h"
#include "lightwindows.h"
#include <gl/GL.h>

void GLEZ_clearColor(f32 r, f32 g, f32 b, f32 a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}