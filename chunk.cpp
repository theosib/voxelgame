

#include "chunk.hpp"
#include <string.h>

Chunk::Chunk(ChunkPos p) : pos(p)
{
    memset(block_storage, 0, sizeof(block_storage));
}

Chunk::~Chunk()
{
    for (int i=0; i<storage_size; i++) {
        Block* block = block_storage[i];
        if (block) delete block;
    }
}

void Chunk::setBlock(BlockPos pos, Block *block)
{
    uint32_t index = chunkBlockIndex(pos);
    Block *old = block_storage[index];
    if (old) delete old;
    block_storage[index] = block;
}


void Chunk::iterate_blocks(MeshData *mesh, Texture *tex)
{
    int x, y, z;
    for (int i=0; i<storage_size; i++) {
        Block* block = block_storage[i];
        if (block && (!tex || block->getShape()->getTexture() == tex)) {
            int x, y, z;
            decodeIndex(i, x, y, z);
            x += pos.X << 4;
            y += pos.Y << 4;
            z += pos.Z << 4;
            BlockPos blockpos(x, y, z);
            
            block->getVertices(mesh->vertices, blockpos);
            block->getTexCoords(mesh->texcoords);
            block->getNormals(mesh->normals);
            mesh->total_vertices += block->numVertices();
        }
    }
}