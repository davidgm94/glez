#include <glez_gl/gl.h>
#include <string.h>
#include <stb_sprintf.h>
#include <Windows.h>
#include <stdio.h>
#include <glez/file.h>
#include <glez/tex.h>

u_opengl_handle gl_gen_texture(texture_info tex, bool transparency)
{
    GLenum format = transparency ? GL_RGBA : GL_RGB;
    u_opengl_handle gl_texture_handle;
    glGenTextures(1, &gl_texture_handle);
    glBindTexture(GL_TEXTURE_2D, gl_texture_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return gl_texture_handle;
}

s_opengl_handle compile_shader(const char* filename, GLenum shader_type)
{
    char shader_relative_path[256] = "src/shaders/";
    strcat(shader_relative_path, filename);
    char* shader_content = load_file(shader_relative_path);
    s_opengl_handle shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char* const*)&shader_content, NULL);
    glCompileShader(shader);
    s32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) {
        printf("Shader %s compiled successfully.\n", filename);
    }
    else {
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
    }
    else {
        char info_log[512];
        glGetProgramInfoLog(shader_program, COUNT_OF(info_log), NULL, info_log);
        printf("Shader program failed to be linked:\n%s\n", info_log);
        _exit(success);
    }

    return shader_program;
}

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
#if GLM_DEBUG
#else
#endif
