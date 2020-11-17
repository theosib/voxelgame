#ifndef _INCLUDED_CHUNK_HPP
#define _INCLUDED_CHUNK_HPP

#include "block.hpp"
#include "position.hpp"
#include "texture.hpp"
#include "mesh.hpp"

class Chunk {
public:
    static const int storage_size = 4096;
    
private:
    ChunkPos pos;
    Block* block_storage[storage_size];
    
    static uint32_t chunkBlockIndex(BlockPos pos) {
        uint32_t x = pos.X & 15;
        uint32_t y = pos.Y & 15;
        uint32_t z = pos.Z & 15;
        return x | (z<<4) | (y<<8);
    }
    
    void decodeIndex(uint32_t index, int& x, int& y, int& z) {
        x = index & 15;
        z = (index >> 4) & 15;
        y = (index >> 8) & 15;
    }
    
public:
    Chunk(ChunkPos p);
    ~Chunk();
    
    Block* getBlock(BlockPos pos) {
        Block *block = block_storage[chunkBlockIndex(pos)];
        // if (!block) return AirBlock.instance();
        return block;
    }
    
    void setBlock(BlockPos pos, Block *block);
    
    Block* indexBlock(int index) { return block_storage[index]; }
    
    void iterate_blocks(MeshData *mesh, Texture *tex);
};

#endif