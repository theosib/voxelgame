

#include "world.hpp"


World World::instance;


Chunk* World::loadChunk(ChunkPos pos)
{
    Chunk *chunk = new Chunk(pos);
    uint64_t packed = pos.packed();
    chunk_storage[packed] = chunk;
    last_chunk_pos = packed;
    last_chunk = chunk;
    return chunk;
}

void World::setBlock(BlockPos pos, Block* block)
{
    Chunk *chunk = getChunk(pos.getChunkPos());
    chunk->setBlock(pos, block);    
    setShowFaces(pos, block);
}

void World::setShowFaces(BlockPos pos, Block* block)
{
    for (int face=0; face<facing::NUM_FACES; face++) {
        Block *neighbor = getNeighbor(pos, face);
        block->updateFaceVisibility(face, neighbor);
        if (neighbor) neighbor->updateFaceVisibility(facing::oppositeFace(face), block);
    }
}

void World::iterate_blocks(MeshData *mesh, Texture *tex)
{
    for (auto i=chunk_storage.begin(); i!=chunk_storage.end(); ++i) {
        Chunk *chunk = i->second;
        chunk->iterate_blocks(mesh, tex);
    }
}

Block* World::getNeighbor(BlockPos pos, int face)
{
    BlockPos npos = pos.neighbor(face);
    return getBlock(npos);
}
