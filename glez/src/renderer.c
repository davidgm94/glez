#include "type.h"
#include "renderer.h"
#include "lightwindows.h"
#include "openGL/gl.h"
#include "model.h"
#include "openGL/glad.h"

GLEZ_API void GLEZ_clearColor(f32 r, f32 g, f32 b, f32 a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLEZ_loadTriangle(void)
{
	S_OpenGLHandle vertexShader = GL_CompileShader("triangle_vert.glsl", GL_VERTEX_SHADER);
	S_OpenGLHandle fragmentShader = GL_CompileShader("triangle_frag.glsl", GL_FRAGMENT_SHADER);
	S_OpenGLHandle program = GL_CreateProgram(vertexShader, fragmentShader);

	SimpleMesh triangleMesh = {0};
}

GLEZ_API void GLEZ_renderTriangle(void)
{
	return;
}
