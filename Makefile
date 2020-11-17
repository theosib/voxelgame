
CXX = clang++
CFLAGS = -g -std=c++11 -Iglm/
CXXFLAGS = $(CFLAGS)
LDFLAGS = $(CFLAGS) -lglfw -framework OpenGL

# SOURCES = glfw.cpp shader.cpp stb.cpp texture.cpp camera.cpp gamecamera.cpp cubedata.cpp
SOURCES = main.cpp stb.cpp texture.cpp window.cpp gamewindow.cpp camera.cpp gamecamera.cpp cubedata.cpp shader.cpp world.cpp block.cpp chunk.cpp facing.cpp position.cpp mesh.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: game

game: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) game

