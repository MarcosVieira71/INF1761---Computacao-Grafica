
TARGET = main

all: $(TARGET)

INC = -I.
LIB = -L.

CPP = g++
#CPPFLAGS = -O2 -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 

SRC = \
  error.cpp \
  shader.cpp \
  main.cpp \
  triangle.cpp

OBJ = $(addsuffix .o, $(basename $(SRC))) 

.cpp.o:
	$(CPP) $(INC) $(CPPFLAGS) -c $< -o $@ 

$(TARGET): $(OBJ) Makefile 
	$(CPP) $(LIB) -o $@ $(OBJ) -lstdc++ -lGL -lGLEW -lglfw -ldl -lpthread -lX11 -lXrandr 


clean:
	rm -f $(TARGET) $(OBJ)


