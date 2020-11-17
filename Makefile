
CXX = clang++
CFLAGS = -g -std=c++11 -Iglm/
CXXFLAGS = $(CFLAGS)
LDFLAGS = $(CFLAGS) -lglfw -framework OpenGL

HEADERS = block.hpp camera.hpp chunk.hpp compat.hpp constants.hpp cubedata.hpp facing.hpp gamecamera.hpp gamewindow.hpp mesh.hpp position.hpp shader.hpp texture.hpp window.hpp world.hpp 
SOURCES = main.cpp stb.cpp texture.cpp window.cpp gamewindow.cpp camera.cpp gamecamera.cpp cubedata.cpp shader.cpp world.cpp block.cpp chunk.cpp facing.cpp position.cpp mesh.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: game

game: $(OBJECTS) $(HEADERS)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) game

