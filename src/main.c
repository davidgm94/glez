#include "type.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <xmmintrin.h>
#include <assert.h>

typedef struct vec4 {
    union {
        __m128 simd;
        float f32[4];
        u32 uns32[4];
    };
} vec4;

typedef enum {
    GLFW_INIT_FAILED = -1,
} FIRST_GAME_ERROR_CODE;


void glfw_error_callback(int error, const char* desc)
{
    frame_logger("[GLFW ERROR %d] %s\n", error, desc);
}

static inline float generate_random_color(void)
{
    return (float)(rand() % 10) * 0.1f;
}

#include "ext/stb_sprintf.h"
#include "sys.h"
#include "file.h"

typedef int s_opengl_handle;
typedef unsigned u_opengl_handle;
int main(int argc, char* argv[])
{
    int glfw_init = glfwInit();
    if (!glfw_init == GLFW_TRUE) {
        const char glfw_init_failed[] = "GLFW initialization failed! Exiting...\n";
        LOG_ERROR_AND_EXIT(glfw_init_failed);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 576, "FirstGame", NULL, NULL);
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

    char* vertex_shader_source = load_file("src/shaders/triangle.glsl");
    s_opengl_handle vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    

    printf("%s\n", file);

    while (!glfwWindowShouldClose(window))
    {
        BEGIN_TIME_BLOCK(TIME_FRAME_TOTAL);
        glfwPollEvents();
        glClearColor(generate_random_color(), generate_random_color(), generate_random_color(), generate_random_color());
        glClear(GL_COLOR_BUFFER_BIT);
        BEGIN_TIME_BLOCK(TIME_FRAME_GPU);
        glfwSwapBuffers(window);
        END_TIME_BLOCK(TIME_FRAME_GPU);
        END_TIME_BLOCK(TIME_FRAME_TOTAL);

        frame_log_and_clear();
        // TODO: this cripples performance when moving the window, use with care
        //  frame_set_window_title(window);
    }

    return 0;
}
