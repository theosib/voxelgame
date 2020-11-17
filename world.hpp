#ifndef _INCLUDED_WORLD_HPP
#define _INCLUDED_WORLD_HPP

#include <unordered_map>
#include "position.hpp"
#include "block.hpp"
#include "chunk.hpp"
#include "texture.hpp"
#include "mesh.hpp"

class World {
public:
    static World instance;
    
private:
    std::unordered_map<uint64_t, Chunk *> chunk_storage;
    uint64_t last_chunk_pos;
    Chunk *last_chunk;
    
    Chunk* loadChunk(ChunkPos pos);
    
    void setShowFaces(BlockPos pos, Block* block);
    
public:
    World() : last_chunk_pos(0), last_chunk(0) {}
    
    Chunk* getChunk(ChunkPos pos) {
        uint64_t packed = pos.packed();
        if (packed == last_chunk_pos && last_chunk) return last_chunk;
        auto i = chunk_storage.find(packed);
        if (i != chunk_storage.end()) {
            last_chunk_pos = packed;
            last_chunk = i->second;
            return last_chunk;
        }
        return loadChunk(pos);
    }
    
    Block* getBlock(BlockPos pos) {
        return getChunk(pos.getChunkPos())->getBlock(pos);
    }
    
    Block* getNeighbor(BlockPos pos, int face);
    
    void setBlock(BlockPos pos, Block* block);
    
    // const std::unordered_map<uint64_t, Chunk *>& getStorage() {
    //     return chunk_storage;
    // }
    
    void iterate_blocks(MeshData *mesh, Texture *tex);
};

#endif