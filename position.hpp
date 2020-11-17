#ifndef _INCLUDED_TYPES_HPP
#define _INCLUDED_TYPES_HPP

#include <string>
#include "constants.hpp"
#include "facing.hpp"



struct ChunkPos {
    int32_t X, Y, Z;
    
    ChunkPos(int32_t x, int32_t y, int32_t z) : X(x), Y(y), Z(z) {}
    
    uint64_t packed() {
        uint64_t x = X & packing::chunkX_mask;
        uint64_t y = Y & packing::chunkY_mask;
        uint64_t z = Z & packing::chunkZ_mask;
        return (x << packing::chunkX_shift) | (y << packing::chunkY_shift) | (z << packing::chunkZ_shift);
    }
    
    std::string toString();
};

struct BlockPos {
    int32_t X, Y, Z;
    
    BlockPos(int32_t x, int32_t y, int32_t z) : X(x), Y(y), Z(z) {}
    
    ChunkPos getChunkPos() {
        return ChunkPos(X>>4, Y>>4, Z>>4);
    }
    
    uint64_t packed() {
        uint64_t x = X & packing::blockX_mask;
        uint64_t y = Y & packing::blockY_mask;
        uint64_t z = Z & packing::blockZ_mask;
        return (x << packing::blockX_shift) | (y << packing::blockY_shift) | (z << packing::blockZ_shift);
    }    
    
    BlockPos down(int dist=1) { return BlockPos(X, Y-dist, Z); }
    BlockPos up(int dist=1) { return BlockPos(X, Y+dist, Z); }
    BlockPos north(int dist=1) { return BlockPos(X, Y, Z-dist); }
    BlockPos south(int dist=1) { return BlockPos(X, Y, Z+dist); }
    BlockPos west(int dist=1) { return BlockPos(X-dist, Y, Z); }
    BlockPos east(int dist=1) { return BlockPos(X+dist, Y, Z); }
    
    BlockPos neighbor(int face) {
        const int *vec = facing::int_vector[face];
        return BlockPos(X+vec[0], Y+vec[1], Z+vec[2]);
    }
    
    std::string toString();
};

#endif