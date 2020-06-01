#pragma once
#include "type.h"
#include "glad.h"
typedef int S_OpenGLHandle;
typedef unsigned U_OpenGLHandle;

typedef struct{
    u32 type;
    u32 count;
    u32 size;
} UniformMetadata;

struct TextureInfo;

U_OpenGLHandle GL_GenerateTexture(struct TextureInfo tex, bool transparency);

S_OpenGLHandle GL_CompileShader(const char* filename, u32 shader_type);
S_OpenGLHandle GL_CreateProgram(S_OpenGLHandle vertex_shader, S_OpenGLHandle fragment_shader);
void GL_FillVertexAttributes(UniformMetadata* element_metadata_arr, u32 element_count, size_t type_size);

void GL_ShaderSetBool(S_OpenGLHandle shader_program, const char* name, bool value);
void GL_ShaderSetInt(S_OpenGLHandle shader_program, const char* name, s32 value);
void GL_ShaderSetFloat(S_OpenGLHandle shader_program, const char* name, f32 value);
#if GLM_DEBUG
void GL_ShaderSetMat4f(s_opengl_handle shader_program, const char* name, mat4 m);
#else
void GL_ShaderSetMat4f(S_OpenGLHandle shader_program, const char* name, mat4f m);
#endif

