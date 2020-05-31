#include "tex.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include "stb/stb_image.h"

TextureInfo textureLoad(const char* filename, bool flipVertically)
{
	stbi_set_flip_vertically_on_load(flipVertically);
	TextureInfo tex;
	tex.data = stbi_load(filename, &tex.width, &tex.height, &tex.channel_count, 0);
	assert(tex.data);
	return tex;
}

void* textureFree(TextureInfo* tex)
{
	stbi_image_free(tex->data);
	return tex->data;
}
