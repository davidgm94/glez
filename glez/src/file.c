#include "type.h"
#include <stdio.h>
#include <assert.h>
#if _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include "file.h"
#endif

typedef struct OsFile
{
    void* handle;
} OsFile;

typedef struct OsAsyncFileOperation
{
    void* handle;
} OsAsyncFileOperation;

static char file_buffer[MEGABYTES(10)];
static char* file_buffer_ptr = file_buffer;

char* fileLoad(const char *name) {
#ifdef GLEZ_DEBUG
    printf("[FILE] Reading file '%s'...\n", name);
    //printf("[FILE] Reading file '%s'...\n", name);
    char cwd[1024];
#if _WIN32
    DWORD char_count = GetCurrentDirectoryA(1024, cwd);
    assert(char_count);
#elif __linux__
    getcwd(cwd, sizeof(cwd));
#endif
    printf("[FILE] Current working directory: %s\n", cwd);
#endif
    FILE* file = fopen(name, "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    printf("[FILE] File length: %ld.\n", length);
    assert(0 < length && (file_buffer_ptr + length) < file_buffer + COUNT_OF(file_buffer));
    fseek(file, 0, SEEK_SET);

    size_t rc = fread(file_buffer_ptr, 1, length, file);
    assert(rc == (size_t)length);
    fclose(file);

    char* file_content = file_buffer_ptr;
    file_buffer_ptr += length;
    return file_content;
}

size_t File_Read(OsFile* file, void* buffer, size_t length)
{
    return 0;
}

size_t File_Write(OsFile* file, const void* buffer, size_t length)
{
    return 0;
}

void File_Seek(OsFile* file, size_t position)
{
}

void File_SeekToEnd(OsFile* file)
{
}

void File_Skip(OsFile* file, size_t bytes)
{
}

size_t File_Tell(const OsFile* file)
{
    return 0;
}

OsAsyncFileOperation File_ReadAsync(OsFile* file, void* buffer, size_t length)
{
    return (OsAsyncFileOperation){ 0 };
}

OsAsyncFileOperation File_WriteAsync(OsFile* file, void* buffer, size_t length)
{
    return (OsAsyncFileOperation){ 0 };
}
