#pragma once
#include <glad/glad.h>
#include <glez/type.h>
typedef int s_opengl_handle;
typedef unsigned u_opengl_handle;

typedef struct{
    GLenum type;
    u32 count;
    u32 size;
} uniform_metadata;

struct texture_info;

u_opengl_handle gl_gen_texture(struct texture_info tex, bool transparency);

s_opengl_handle compile_shader(const char* filename, GLenum shader_type);
s_opengl_handle create_program(s_opengl_handle vertex_shader, s_opengl_handle fragment_shader);
void fill_vertex_attributes(uniform_metadata* element_metadata_arr, u32 element_count, size_t type_size);

inline void shader_set_bool(s_opengl_handle shader_program, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), (s32)value);
}

inline void shader_set_int(s_opengl_handle shader_program, const char* name, s32 value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}

inline void shader_set_float(s_opengl_handle shader_program, const char* name, f32 value)
{
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}

//void shader_set_mat4f(s_opengl_handle shader_program, const char* name, mat4f m)
//{
//    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*) &m);
//}
#if GLM_DEBUG
inline void shader_set_mat4(s_opengl_handle shader_program, const char* name, mat4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*)m);
}
#else
inline void shader_set_mat4(s_opengl_handle shader_program, const char* name, mat4f m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*)&m);
}
#endif

