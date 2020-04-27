#include "tex.h"
#include <assert.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

texture_info load_texture(const char* filename, bool flip_vertically)
{
	stbi_set_flip_vertically_on_load(flip_vertically);
	texture_info tex;
	tex.data = stbi_load(filename, &tex.width, &tex.height, &tex.channel_count, 0);
	assert(tex.data);
	return tex;
}

void* free_texture(texture_info* tex)
{
	stbi_image_free(tex->data);
	return tex->data;
}
