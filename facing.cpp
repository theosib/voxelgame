

#include "facing.hpp"
#include <ctype.h>

// enum direction {
//     DOWN,   // negative Y
//     UP,     // positive Y
//     NORTH,  // negative Z
//     SOUTH,  // positive Z
//     WEST,   // negative X
//     EAST    // positive X
// };

namespace facing {
    const glm::vec3 float_vector[] = {
        glm::vec3(0.0, -1.0, 0.0),  // DOWN
        glm::vec3(0.0, 1.0, 0.0),   // UP
        glm::vec3(0.0, 0.0, -1.0),  // NORTH
        glm::vec3(0.0, 0.0, 1.0),   // SOUTH
        glm::vec3(-1.0, 0.0, 0.0),  // WEST
        glm::vec3(1.0, 0.0, 0.0)    // EAST
    };
    
    const int int_vector[6][3] {
        {0, -1, 0},
        {0, 1, 0},
        {0, 0, -1},
        {0, 0, 1},
        {-1, 0, 0},
        {1, 0, 0}
    };
    
    int faceFromName(const char *name)
    {
        switch (tolower(name[0])) {
        case 'd':
        case 'b':
            return DOWN;
        case 'u':
        case 't':
            return UP;
        case 'n':
            return NORTH;
        case 's':
            return SOUTH;
        case 'w':
            return WEST;
        case 'e':
            return EAST;
        default:
            return -1;
        }
    }
}
