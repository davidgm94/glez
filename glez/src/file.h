#pragma once

#include "type.h"
typedef struct OsFile OsFile;
typedef struct OsAsyncFileOperation OsAsyncFileOperation;
char* fileLoad(const char* filename);

// SYNC API
size_t File_Read(OsFile* file, void* buffer, size_t length);
size_t File_Write(OsFile* file, const void* buffer, size_t length);
void File_Seek(OsFile* file, size_t position);
void File_SeekToEnd(OsFile* file);
void File_Skip(OsFile* file, size_t bytes);
size_t File_Tell(const OsFile* file);

// ASYNC API
OsAsyncFileOperation File_ReadAsync(OsFile* file, void* buffer, size_t length);
OsAsyncFileOperation File_WriteAsync(OsFile* file, void* buffer, size_t length);
