#include "type.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include "game_math.h"

void glfw_error_callback(int error, const char* desc)
{
    frame_logger("[GLFW ERROR %d] %s\n", error, desc);
}

static inline f32 generate_random_color(void)
{
    return (f32)(rand() % 10) * 0.1f;
}

#include "../ext/stb_sprintf.h"
#include "sys.h"
#include "file.h"

typedef int s_opengl_handle;
typedef unsigned u_opengl_handle;

s_opengl_handle compile_shader(const char* filename, GLenum shader_type)
{
    char shader_relative_path[1024] = "src/shaders/";
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

typedef struct {
    u32 width, height;
} window_dimension;


typedef struct {
    f32 position[3];
    f32 color[3];
} f32_pos_f32_color_vertex;

typedef struct {
    f32 position[3];
} pos_vertex;

typedef struct{
    GLenum type;
    u8 count;
    u8 size;
    u8 offset;
    u8 padding;
} uniform_metadata;

void fill_vertex_attributes(uniform_metadata* element_metadata_arr, u32 element_count, size_t type_size)
{
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), NULL);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)(3 * sizeof(f32)));
    // glEnableVertexAttribArray(1);
    u8 offset = 0;
    for (int i = 0; i < element_count; i++)
    {
        uniform_metadata metadata_i = element_metadata_arr[i];
        //printf("arg0: %d, arg1: %d, type..., arg_penul: %d, arg_last: %d\n", i, metadata_i.count, metadata_i.count * metadata_i.size, )
        glVertexAttribPointer(i, metadata_i.count, metadata_i.type, GL_FALSE, type_size, (void*)offset);
        glEnableVertexAttribArray(i);
        offset += metadata_i.count * metadata_i.size;
    }
}

window_dimension w_dimension = {
        .width = 1024,
        .height = 576,
};

void framebuffer_size_callback(GLFWwindow* window)
{
    glViewport(0, 0, w_dimension.width, w_dimension.height);
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

int main(int argc, char* argv[])
{
    printf("%zu\n", sizeof(vec3f));
//    vec4f a = {1.24f, 213.0f, 012.0f, 1293.0f};
//    vec4f b = {1.24f, 2.0f, 1.0f, 12.0f};
//    vec4f c = add(a, b);
//    print_vec4f(c);
//    __m128 a_ = {1.24f, 213.0f, 012.0f, 1293.0f};
//    __m128 b_ = {1.24f, 2.0f, 1.0f, 12.0f};
//    __m128 c_ = _mm_add_ss(a_, b_);
//    print_vec4f(c_);
//    exit(0);

    const char* w_title = "FirstGame";

    int glfw_init = glfwInit();
    if (!(glfw_init == GLFW_TRUE)) {
        const char glfw_init_failed[] = "GLFW initialization failed! Exiting...\n";
        LOG_ERROR_AND_EXIT(glfw_init_failed);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(w_dimension.width, w_dimension.height, w_title, NULL, NULL);
    if (!window) {
        const char window_err_msg[] = "GLFW window couldn't be created\n";
        LOG_ERROR_AND_EXIT(window_err_msg);
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(glfw_error_callback);
    int glad_init = gladLoadGL();
    if (!glad_init) {
        const char glad_err_msg[] = "GLAD initialization failed. Exiting...\n";
        LOG_ERROR_AND_EXIT(glad_err_msg);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    s_opengl_handle vertex_shader = compile_shader("shaders_vert.glsl", GL_VERTEX_SHADER);
    s_opengl_handle fragment_shader = compile_shader("shaders_frag.glsl", GL_FRAGMENT_SHADER);
    s_opengl_handle shader_program = create_program(vertex_shader, fragment_shader);

    const f32 scale = 0.5f;
    const f32 color = 1.0f;
    pos_vertex vertices[4] = {
            [0].position = { scale, scale, 0.0f },
            [1].position = { scale, -scale, 0.0f },
            [2].position = { -scale, -scale, 0.0f },
            [3].position = { -scale, scale, 0.0f },
    };

    u32 indices[] =
            {
            0, 1, 3,
            1, 2, 3,
            };

    u_opengl_handle vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    uniform_metadata metadata[2] = {
            [0] = {
                    .type = GL_FLOAT,
                    .count = 3,
                    .size = sizeof(f32),
                    .offset = my_offsetof(f32_pos_f32_color_vertex, position)
            },
            // [1] = {
            //         .type = GL_FLOAT,
            //         .count = 3,
            //         .size = sizeof(f32),
            //         .offset = my_offsetof(f32_pos_f32_color_vertex, color)
            // },
    };

    fill_vertex_attributes(metadata, COUNT_OF(metadata), sizeof(pos_vertex));

    /// glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        BEGIN_TIME_BLOCK(TIME_FRAME_TOTAL);
        frame_log_and_clear();
        glfwPollEvents();
        BEGIN_TIME_BLOCK(TIME_FRAME_GPU);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // f32_4x4 transform = mat4_get_identity();
        // transform = mat4f_translate_vec3(transform, (vec3f){0.5f, -0.5f, 0.0f});


        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        END_TIME_BLOCK(TIME_FRAME_GPU);
        END_TIME_BLOCK(TIME_FRAME_TOTAL);

        // TODO: this cripples performance when moving the window, use with care
        //  frame_set_window_title(window);
    }

    return 0;
}
