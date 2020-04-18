#pragma once
#include <glad/glad.h>
typedef int s_opengl_handle;
typedef unsigned u_opengl_handle;

s_opengl_handle compile_shader(const char* filename, GLenum shader_type)
{
    char shader_relative_path[256] = "src/shaders/";
    strcat(shader_relative_path, filename);
    char* shader_content = load_file(shader_relative_path);
    s_opengl_handle shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char *const *)&shader_content, NULL);
    glCompileShader(shader);
    s32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) {
        printf("Shader %s compiled successfully.\n", filename);
    } else {
        char info_log[512];
        glGetShaderInfoLog(shader, COUNT_OF(info_log), NULL, info_log);
        printf("Shader %s failed to compile:\n%s\n", filename, info_log);
        _exit(success);
    }
    return shader;
}

s_opengl_handle create_program(s_opengl_handle vertex_shader, s_opengl_handle fragment_shader)
{
    s_opengl_handle shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    s32 success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (success) {
        printf("Shader program linked successfully.\n");
    } else {
        char info_log[512];
        glGetProgramInfoLog(shader_program, COUNT_OF(info_log), NULL, info_log);
        printf("Shader program failed to be linked:\n%s\n", info_log);
        _exit(success);
    }

    return shader_program;
}

typedef struct{
    GLenum type;
    u8 count;
    u8 size;
    u8 offset;
    u8 padding[1];
} uniform_metadata;

void fill_vertex_attributes(uniform_metadata* element_metadata_arr, u32 element_count, size_t type_size)
{
    u8 offset = 0;
    for (int i = 0; i < element_count; i++)
    {
        uniform_metadata metadata_i = element_metadata_arr[i];
        //printf("arg0: %d, arg1: %d, type..., arg_penul: %d, arg_last: %d\n", i, metadata_i.count, metadata_i.count * metadata_i.size, )
        glVertexAttribPointer(i, metadata_i.count, metadata_i.type, GL_FALSE, type_size, (void*)(u64)offset);
        glEnableVertexAttribArray(i);
        offset += metadata_i.count * metadata_i.size;
    }
}

void shader_set_bool(s_opengl_handle shader_program, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), (s32)value);
}

void shader_set_int(s_opengl_handle shader_program, const char* name, s32 value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}

void shader_set_float(s_opengl_handle shader_program, const char* name, f32 value)
{
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}

//void shader_set_mat4f(s_opengl_handle shader_program, const char* name, mat4f m)
//{
//    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*) &m);
//}
#if GLM_DEBUG
void shader_set_mat4(s_opengl_handle shader_program, const char* name, mat4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*)m);
}
#else
void shader_set_mat4(s_opengl_handle shader_program, const char* name, mat4f m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*)&m);
}
#endif

