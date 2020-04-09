#include <stdio.h>
#include <assert.h>
#include "type.h"

static char file_buffer[MEGABYTES(1024)];
static char* file_buffer_ptr = file_buffer;

char* load_file(const char *name) {
    FILE* file = fopen(name, "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    assert(0 < length && (file_buffer_ptr + length) < file_buffer + COUNT_OF(file_buffer));
    fseek(file, 0, SEEK_SET);

    size_t rc = fread(file_buffer_ptr, 1, length, file);
    assert(rc == (size_t)length);
    fclose(file);

    char* file_content = file_buffer_ptr;
    file_buffer_ptr += length;
    return file_content;
}
