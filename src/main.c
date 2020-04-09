#include "type.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <xmmintrin.h>

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

int main(int argc, char* argv[])
{
    int glfw_init = glfwInit();
    if (glfw_init == GLFW_TRUE) {
    } else {
        _exit(GLFW_INIT_FAILED);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    GLFWwindow* window = glfwCreateWindow(1024, 576, "FirstGame", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(glfw_error_callback);
    int glad_init = gladLoadGL();
    if (!glad_init) {
        const char glad_err_msg[] = "GLAD initialization failed. Exiting...\n";
        LOG_ERROR_AND_EXIT(glad_err_msg);
    }

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
