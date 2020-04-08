#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <GLFW/glfw3.h>
#define STB_SPRINTF_IMPLEMENTATION
#include "ext/stb_sprintf.h"
typedef uint8_t byte;
typedef uint32_t u32;
typedef uint64_t u64;
#define COUNT_OF(___arr) ((sizeof(___arr)) / (sizeof(___arr[0]))
#define BYTES(n) 	 ((u64)n)
#define KILOBYTES(n) (1024 * BYTES(n))
#define MEGABYTES(n) (1024 * KILOBYTES(n))
#define GIGABYTES(n) (1024 * MEGABYTES(n))
#define TERABYTES(n) (1024 * GIGABYTES(n))

#define FRAME_STRING_BUFFER_CHAR_COUNT (MEGABYTES(5))
static char _g_frame_string_buffer[FRAME_STRING_BUFFER_CHAR_COUNT];
#define PRINT_STRING_BUFFER_CHAR_COUNT (MEGABYTES(5))
static char _g_print_string_buffer[PRINT_STRING_BUFFER_CHAR_COUNT];
static char* _g_print_string_buffer_ptr = _g_print_string_buffer;
static char* _g_frame_string_buffer_ptr = _g_frame_string_buffer;

#define INLINE_FORMAT_AND_APPEND_TO_PRINT_BUFFER(fmt, ...) do { va_list args; va_start(fmt, args); int bytes_written = stbsp_vsprintf(_g_print_string_buffer_ptr, fmt, args); va_end(args); _g_print_string_buffer_ptr += (u32)bytes_written; } while(0)
static void format_and_append_to_print_buffer(char* fmt, ...)
{
    va_list args;
    va_start(fmt, args);
    int bytes_written = stbsp_vsprintf(_g_print_string_buffer_ptr, fmt, args);
    va_end(args);
    printf("%.*s\n", bytes_written, _g_print_string_buffer_ptr);
    _g_print_string_buffer_ptr += (u32)bytes_written;
}

static void format_and_append_to_frame_buffer(char* fmt, ...)
{
    va_list args;
    va_start(fmt, args);
    int bytes_written = stbsp_vsprintf(_g_frame_string_buffer_ptr, fmt, args);
    va_end(args);
    _g_frame_string_buffer_ptr += (u32)bytes_written;
}

static void append_to_print_buffer(char* str)
{
    size_t len = strlen(str);
    strcpy(_g_print_string_buffer_ptr, str);
    _g_print_string_buffer_ptr += len;
}

static void append_to_frame_buffer(char* str)
{
    size_t len = strlen(str);
    strcpy(_g_frame_string_buffer_ptr, str);
    _g_frame_string_buffer_ptr += len;
}

static inline clear_buffers(void)
{
    _g_frame_string_buffer_ptr = _g_frame_string_buffer;
    _g_print_string_buffer_ptr = _g_print_string_buffer;
}

typedef enum {
    GLFW_INIT_FAILED = -1,
} FIRST_GAME_ERROR_CODE;

typedef enum {
    INFO,
    ERROR,
    CRITICAL,
} LOG_LEVEL;

#define my_printf(str) fwrite(str, strlen(str), 1, stdout)
#define my_printf_len_known(str, len) fwrite(str, len, 1, stdout)
#define my_printf_unlocked(str) fwrite_unlocked(str, strlen(str), 1, stdout)
#define my_printf_len_known_unlocked(str, len) fwrite_unlocked(str, len, 1, stdout)
#define my_vprintf()
#define logger(_log_level, ___fmt, ...) logger_wrapper((_log_level), (__FILE__), (__LINE__), (___fmt))
#define vlogger(_log_level, ___fmt, ...) vlogger_wrapper((_log_level), (__FILE__), (__LINE__), ___fmt, __VA_ARGS__)
#define future(_log_level, __fmt, ...) do {\
    static const char* info = "INFO";\
    static const char* error = "ERROR";\
    static const char* critical = "CRITICAL";\
    const char* level;\
    switch (log_level) {\
    case INFO:\
    level = info;\
    break;\
    case ERROR:\
    level = error;\
    break;\
    case CRITICAL:\
    level = critical;\
    break;\
time_t now_time_t = time(NULL);\
struct tm now = *localtime(&now_time_t);\

} while(0))

static void vlogger_wrapper(LOG_LEVEL log_level, const char* file, size_t line, const char* message_format, ...) {
    static const char* info = "INFO";
    static const char* error = "ERROR";
    static const char* critical = "CRITICAL";
    const char* level;
    switch (log_level) {
        case INFO:
            level = info;
            break;
        case ERROR:
            level = error;
            break;
        case CRITICAL:
            level = critical;
            break;
    }

    time_t now_time_t = time(NULL);
    struct tm now = *localtime(&now_time_t);

    char buffer[4096];
    va_list args;
    va_start(args, message_format);
    vprintf("\n%s %s\n", args);
    int bytes_written_to_buffer = stbsp_sprintf(buffer, message_format, args);
    printf("[TEST] buffer: %.*s\n", bytes_written_to_buffer, buffer);
    printf("buffer: %s\n", buffer);
    va_end(args);

    int bytes_written = stbsp_sprintf(_g_print_string_buffer_ptr, "[%d-%02d-%02d %02d:%02d:%02d] [%s] [%s:%zu] %s\n", 1900 + now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, level, file, line, buffer);
    _g_print_string_buffer_ptr += bytes_written;
}

static void logger_wrapper(LOG_LEVEL log_level, const char* file, size_t line, const char* str) {
    static const char* info = "INFO";
    static const char* error = "ERROR";
    static const char* critical = "CRITICAL";
    const char* level;
    switch (log_level) {
        case INFO:
            level = info;
            break;
        case ERROR:
            level = error;
            break;
        case CRITICAL:
            level = critical;
            break;
    }

    time_t now_time_t = time(NULL);
    struct tm now = *localtime(&now_time_t);

    int bytes_written = stbsp_sprintf(_g_print_string_buffer_ptr, "[%d-%02d-%02d %02d:%02d:%02d] [%s] [%s:%zu] %s\n", 1900 + now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, level, file, line, str);
    _g_print_string_buffer_ptr += bytes_written;
}

void foo(const char* format, ...)
{
    va_list args;
    va_start(format, args);
    vprintf(format, args);
    va_end(args);
}

int main(int argc, char* argv[])
{
    foo("%s", "Hello", NULL);
    //int glfwInitialization = glfwInit();
    //if (glfwInitialization == GLFW_TRUE) {
    //    printf("GLFW initialised properly\n");
    //} else {
    //    printf("GLFW initialised poorly\n");
    //    _exit(GLFW_INIT_FAILED);
    //}


    //for (int i = 0; i < 10000; i++)
    //{
    //    logger(INFO, "Hi");
    //    vlogger(INFO, "Hello", "world", "hi");

    //    fwrite(_g_print_string_buffer, _g_print_string_buffer_ptr - _g_print_string_buffer, 1, stdout);
    //    //*_g_print_string_buffer_ptr = 0;
    //    //fwrite_unlocked(_g_print_string_buffer, _g_frame_string_buffer_ptr - _g_print_string_buffer, 1, stdout);
    //    clear_buffers();
    //}

    return 0;
}
