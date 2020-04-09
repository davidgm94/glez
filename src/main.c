#if NDEBUG
#define GAME_RELEASE
#define LOGS 1
#else
#define GAME_DEBUG
#define LOGS 1
#endif
#define FAST_AND_ERROR_PRONE 0
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <errno.h>

#define STB_SPRINTF_IMPLEMENTATION
#include "ext/stb_sprintf.h"
// #include "boilerplate_str.h"
#define POSIX_FREQUENCY 1000000.0
typedef uint8_t byte;
typedef uint32_t u32;
typedef uint64_t u64;
#define COUNT_OF(___arr) ((sizeof(___arr)) / (sizeof(___arr[0]))
#define BYTES(n) 	 ((u64)n)
#define KILOBYTES(n) (1024 * BYTES(n))
#define MEGABYTES(n) (1024 * KILOBYTES(n))
#define GIGABYTES(n) (1024 * MEGABYTES(n))
#define TERABYTES(n) (1024 * GIGABYTES(n))

#define PRINT_STRING_BUFFER_CHAR_COUNT (MEGABYTES(5))
static char _g_print_string_buffer[PRINT_STRING_BUFFER_CHAR_COUNT];
static char* _g_print_string_buffer_ptr = _g_print_string_buffer;

static inline size_t dont_format(char* str)
{
    size_t len = strlen(str);
    for (u64 i = 0; i < len; i++)
    {
        char c = str[i];
        if (c == '%') {
            return 0;
        }
    }

    return len;
}

static void format_and_append_to_print_buffer(char* fmt, ...)
{
#if LOGS
    size_t len = dont_format(fmt);
    if (len) {
#define SHOULD_LOG_WRITTEN_BYTES_INFO 0
#if SHOULD_LOG_WRITTEN_BYTES_INFO
        printf("Bytes written to buffer: %zu\n", len);
#endif
        strcpy(_g_print_string_buffer_ptr, fmt);
        _g_print_string_buffer_ptr += len;
        return;
    }

    va_list args;
    va_start(args, fmt);
    int bytes_written = stbsp_vsprintf(_g_print_string_buffer_ptr, fmt, args);
    va_end(args);
    _g_print_string_buffer_ptr += (u64)bytes_written;
#endif
}

static inline void clear_buffers(void)
{
#if LOGS
#endif
}

typedef enum {
    GLFW_INIT_FAILED = -1,
} FIRST_GAME_ERROR_CODE;

typedef enum {
    INFO,
    ERROR,
    CRITICAL,
} LOG_LEVEL;

#define FOREACH_TIME_BLOCK(TIME_BLOCK)\
    TIME_BLOCK(TIME_FRAME_TOTAL)\
    TIME_BLOCK(TIME_FRAME_CPU)\
    TIME_BLOCK(TIME_FRAME_GPU)\
    /*TIME_BLOCK(TIME_FRAME_UPDATE)*/\
    TIME_BLOCK(TIME_FRAME_ELEMENT_COUNT)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum TIME_BLOCK
{
    FOREACH_TIME_BLOCK(GENERATE_ENUM)
} TIME_BLOCK;

static const char* TIME_BLOCK_STRING[] =
{
    FOREACH_TIME_BLOCK(GENERATE_STRING)
};

typedef struct {
    struct timespec start;
    struct timespec end;
    float ms;
} timing_record;

typedef struct {
    timing_record record[TIME_FRAME_ELEMENT_COUNT];
} frametime_record;

static frametime_record past_frame;
static frametime_record current_frame;

static inline double compute_ms(struct timespec start, struct timespec end)
{
    u64 start_ns = (u64)start.tv_sec * (u64)10e9 + (u64)start.tv_nsec;
    u64 end_ns = (u64)end.tv_sec * (u64)10e9 + (u64)end.tv_nsec;
    u64 ns = end_ns - start_ns;
    return ns / POSIX_FREQUENCY;
}

void glfw_error_callback(int error, const char* desc)
{
    format_and_append_to_print_buffer("[GLFW ERROR %d] %s\n", error, desc);
}

#define BEGIN_TIME_BLOCK(name) clock_gettime(CLOCK_MONOTONIC, &current_frame.record[name].start)
#define END_TIME_BLOCK(name) clock_gettime(CLOCK_MONOTONIC, &current_frame.record[name].end)

void log_and_clear(void)
{
    for (u32 i = 0; i < TIME_FRAME_ELEMENT_COUNT; i++)
    {
        current_frame.record[i].ms = compute_ms(current_frame.record[i].start, current_frame.record[i].end);
    }

    current_frame.record[TIME_FRAME_CPU].ms = current_frame.record[TIME_FRAME_TOTAL].ms - current_frame.record[TIME_FRAME_GPU].ms;
    format_and_append_to_print_buffer("NEW FRAME:\n");
    for (u32 i = 0; i < TIME_FRAME_ELEMENT_COUNT; i++)
    {
        format_and_append_to_print_buffer("\t* [%s] %.02f ms.\n", TIME_BLOCK_STRING[i], current_frame.record[i].ms);
    }
    format_and_append_to_print_buffer("\t* [TIME_FRAME_SUMMARY] CPU: %.02f%%. GPU: %.02f%%\n",
                                      (current_frame.record[TIME_FRAME_CPU].ms / current_frame.record[TIME_FRAME_TOTAL].ms) * 100.0f,
                                      (current_frame.record[TIME_FRAME_GPU].ms / current_frame.record[TIME_FRAME_TOTAL].ms) * 100.0f);
    fwrite(_g_print_string_buffer, _g_print_string_buffer_ptr - _g_print_string_buffer, 1, stdout);
    // Reset the buffer
    _g_print_string_buffer_ptr = _g_print_string_buffer;
}

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
        fwrite("GLAD initialization failed. Exiting...\n", 50, 1, stdout);
        _exit(-1);
    } else {
        format_and_append_to_print_buffer("GLAD init succeeded!\n");
    }

    int profile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    printf("PROFILE: %d", profile);
    if (profile == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) printf("Compatibility");
    else printf ("Core");


    while (!glfwWindowShouldClose(window))
    {
        BEGIN_TIME_BLOCK(TIME_FRAME_TOTAL);
        glfwPollEvents();
        glClearColor((float)(rand() % 10) * 0.1, (float)(rand() % 10) * 0.1,(float)(rand() % 10) * 0.1, (float)(rand() % 10) * 0.1);
        glClear(GL_COLOR_BUFFER_BIT);
        BEGIN_TIME_BLOCK(TIME_FRAME_GPU);
        glfwSwapBuffers(window);
        END_TIME_BLOCK(TIME_FRAME_GPU);
        END_TIME_BLOCK(TIME_FRAME_TOTAL);

        log_and_clear();
    }

    return 0;
}
