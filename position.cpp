#include "position.hpp"


#include <sstream>

std::string BlockPos::toString()
{
    std::stringstream ss;
    ss << "block(" << X << ',' << Y << ',' << Z << ')';
    return ss.str();
}

std::string ChunkPos::toString()
{
    std::stringstream ss;
    ss << "chunk(" << X << ',' << Y << ',' << Z << ')';
    return ss.str();
}
