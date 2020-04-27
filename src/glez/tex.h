#pragma once
#include <glez/type.h>

typedef struct texture_info
{
    void* data;
    s32 width, height, channel_count;
} texture_info;

texture_info load_texture(const char* filename, bool flip_vertically);

void* free_texture(texture_info* tex);
