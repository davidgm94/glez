#pragma once

size_t meshopt_generateVertexRemap(unsigned int* destination, const unsigned int* indices, size_t indexCount, const void* vertices, size_t vertexCount, size_t vertex_size);
void meshopt_remapVertexBuffer(void* destination, const void* vertices, size_t vertexCount, size_t vertex_size, const unsigned int* remap);
void meshopt_remapIndexBuffer(unsigned int* destination, const unsigned int* indices, size_t indexCount, const unsigned int* remap);
