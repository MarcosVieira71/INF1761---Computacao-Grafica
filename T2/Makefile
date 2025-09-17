TARGET = main

CPP = g++
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION
INC = -I.
LIB = -L.
LDFLAGS = -lstdc++ -lGL -lGLEW -lglfw -ldl -lpthread -lX11 -lXrandr

SRC = \
  error.cpp \
  shader.cpp \
  main.cpp \
  Clock.cpp \
  Hand.cpp \
  Circle.cpp \
  
all: $(TARGET)

$(TARGET): $(SRC) Makefile
	$(CPP) $(INC) $(CPPFLAGS) $(SRC) $(LIB) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
