#include "gl.h"
#include "lightwindows.h"
#include "file.h"
#include "tex.h"
#include "logger.h"
#include "glad.h"

extern bool g_Running;

U_OpenGLHandle GL_GenerateTexture(TextureInfo tex, bool transparency)
{
    GLenum format = transparency ? GL_RGBA : GL_RGB;
    U_OpenGLHandle gl_texture_handle;
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

S_OpenGLHandle GL_CompileShader(const char* filename, u32 shader_type)
{
    char shader_relative_path[256] = "src/shaders/";
    strcat(shader_relative_path, filename);
    char* shader_content = fileLoad(shader_relative_path);
    S_OpenGLHandle shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char* const*)&shader_content, NULL);
    glCompileShader(shader);
    s32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        logInfo("Shader %s compiled successfully.\n", filename);
    }
    else
    {
        char info_log[512];
        glGetShaderInfoLog(shader, COUNT_OF(info_log), NULL, info_log);
        logInfo("Shader %s failed to compile:\n%s\n", filename, info_log);
        g_Running = false;
    }
    return shader;
}

S_OpenGLHandle GL_CreateProgram(S_OpenGLHandle vertex_shader, S_OpenGLHandle fragment_shader)
{
    S_OpenGLHandle shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    s32 success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (success)
    {
        logInfo("Shader program linked successfully.\n");
    }
    else
    {
        char info_log[512];
        glGetProgramInfoLog(shader_program, COUNT_OF(info_log), NULL, info_log);
        logInfo("Shader program failed to be linked:\n%s\n", info_log);
        g_Running = false;
    }

    return shader_program;
}

void GL_FillVertexAttributes(UniformMetadata* element_metadata_arr, u32 element_count, size_t type_size)
{
    u8 offset = 0;
    for (int i = 0; i < element_count; i++)
    {
        UniformMetadata metadata_i = element_metadata_arr[i];
        //printf("arg0: %d, arg1: %d, type..., arg_penul: %d, arg_last: %d\n", i, metadata_i.count, metadata_i.count * metadata_i.size, )
        glVertexAttribPointer(i, metadata_i.count, metadata_i.type, GL_FALSE, type_size, (void*)(u64)offset);
        glEnableVertexAttribArray(i);
        offset += metadata_i.count * metadata_i.size;
    }
}
void GL_ShaderSetBool(S_OpenGLHandle shader_program, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), (s32)value);
}
void GL_ShaderSetInt(S_OpenGLHandle shader_program, const char* name, s32 value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}
void GL_ShaderSetFloat(S_OpenGLHandle shader_program, const char* name, f32 value)
{
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}
#if GLM_DEBUG
void GL_ShaderSetMat4f(s_opengl_handle shader_program, const char* name, mat4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*)m);
}
#else
void GL_ShaderSetMat4f(S_OpenGLHandle shader_program, const char* name, mat4f m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*)&m);
}
#endif
