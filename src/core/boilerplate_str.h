#pragma once
// static void format_and_append_to_frame_buffer(char* fmt, ...)
// {
// #if LOGS
//     va_list args;
//     va_start(args, fmt);
//     int bytes_written = stbsp_vsprintf(_g_frame_string_buffer_ptr, fmt, args);
//     va_end(args);
//     _g_frame_string_buffer_ptr += (u64)bytes_written;
// #endif
// }
//
// static void append_to_print_buffer(char* str)
// {
// #if LOGS
//     size_t len = strlen(str);
//     strcpy(_g_print_string_buffer_ptr, str);
//     _g_print_string_buffer_ptr += len;
// #endif
// }
//
// static void append_to_frame_buffer(char* str)
// {
// #if LOGS
//     size_t len = strlen(str);
//     strcpy(_g_frame_string_buffer_ptr, str);
//     _g_frame_string_buffer_ptr += len;
// #endif
// }
// static void vlogger_wrapper(LOG_LEVEL log_level, const char* file, size_t line, const char* message_format, ...) {
//     char buffer[4096];
//     va_list args;
//     va_start(args, message_format);
//     //vprintf("\n%s %s\n", args);
//     int bytes_written_to_buffer = stbsp_vsprintf(buffer, message_format, args);
//     printf("Bytes written: %d\n", bytes_written_to_buffer);
//     for (int i = 0; i < 15; i++)
//     {
//         printf("Buffer+5: %s\n", buffer+i);
//     }
//     va_end(args);
//     static const char* info = "INFO";
//     static const char* error = "ERROR";
//     static const char* critical = "CRITICAL";
//     const char* level;
//     switch (log_level) {
//         case INFO:
//             level = info;
//             break;
//         case ERROR:
//             level = error;
//             break;
//         case CRITICAL:
//             level = critical;
//             break;
//     }
//
//     time_t now_time_t = time(NULL);
//     struct tm now = *localtime(&now_time_t);
//
//
//     int bytes_written = stbsp_sprintf(_g_print_string_buffer_ptr, "[%d-%02d-%02d %02d:%02d:%02d] [%s] [%s:%zu] %s\n", 1900 + now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, level, file, line, buffer);
//     _g_print_string_buffer_ptr += bytes_written;
// }
//
// static void logger_wrapper(LOG_LEVEL log_level, const char* file, size_t line, const char* str) {
//     static const char* info = "INFO";
//     static const char* error = "ERROR";
//     static const char* critical = "CRITICAL";
//     const char* level;
//     switch (log_level) {
//         case INFO:
//             level = info;
//             break;
//         case ERROR:
//             level = error;
//             break;
//         case CRITICAL:
//             level = critical;
//             break;
//     }
//
//     time_t now_time_t = time(NULL);
//     struct tm now = *localtime(&now_time_t);
//
//     int bytes_written = stbsp_sprintf(_g_print_string_buffer_ptr, "[%d-%02d-%02d %02d:%02d:%02d] [%s] [%s:%zu] %s\n", 1900 + now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, level, file, line, str);
//     _g_print_string_buffer_ptr += bytes_written;
// }
//
// void foo(const char* format, ...)
// {
//     va_list args;
//     va_start(args, format);
//     vprintf(format, args);
//     va_end(args);
// }
