TARGET = solar

all: $(TARGET)

INC = -I.
LIB = -L.

CPP = g++
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 

SRC = \
  arcball.cpp \
  camera2d.cpp \
  camera3d.cpp \
  color.cpp \
  cube.cpp \
  error.cpp \
  framebuffer.cpp \
  grid.cpp \
  image.cpp \
  light.cpp \
  material.cpp \
  mesh.cpp \
  node.cpp \
  quad.cpp \
  polyoffset.cpp \
  scene.cpp \
  shader.cpp \
  sphere.cpp \
  state.cpp \
  texbuffer.cpp \
  texcube.cpp \
  texdepth.cpp \
  texture.cpp \
  transform.cpp \
  triangle.cpp \
  main_2d.cpp \

OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

build/%.o: src/%.cpp | build
	$(CPP) $(INC) $(CPPFLAGS) -c $< -o $@

build:
	mkdir -p build

$(TARGET): $(OBJ) Makefile 
	$(CPP) $(LIB) -o $@ $(OBJ) -lstdc++ -lGL -lGLEW -lglfw -ldl -lpthread -lX11 -lXrandr

clean:
	rm -rf build

run: $(TARGET)
	./$(TARGET)



