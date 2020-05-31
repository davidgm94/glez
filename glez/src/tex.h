#pragma once
#include "type.h"

typedef struct TextureInfo
{
    void* data;
    s32 width, height, channel_count;
} TextureInfo;

#ifdef __cplusplus
extern "C" {
#endif
TextureInfo load_texture(const char* filename, bool flip_vertically);
void* free_texture(TextureInfo* tex);
#ifdef __cplusplus
}
#endif
