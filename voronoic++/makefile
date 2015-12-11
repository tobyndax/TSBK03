ifeq ($(shell uname), Darwin)
	PLATFORM=mac
endif
ifeq ($(shell uname), Linux)
	PLATFORM=linux
endif

$SOURCEDIR = src

CPP_FILES := $(wildcard src/*.cpp)

OBJ_FILES_CPP := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

C_FILES_COMMON := $(wildcard src/common/*.c)

CPP_FILES_COMMON := $(wildcard src/common/*.cpp)

OBJ_FILES_COMMON := $(addprefix obj/,$(notdir $(C_FILES_COMMON:.c=.o)))
OBJ_FILES_COMMON += $(addprefix obj/,$(notdir $(CPP_FILES_COMMON:.cpp=.o)))

INC = -Isrc/ -Isrc/common/ -Isrc/common/glm -Isrcqu3e
MAIN = main.cpp
OUT = bin/voronoi

ifeq ($(PLATFORM),mac)
INC += -F/Library/Frameworks/
LINKS =  -framework OpenGL -framework GLUT -framework Cocoa -framework SDL2 -L/usr/local/lib -Llib -lqu3e
endif

GCC = g++ -Wall -pedantic -std=c++11 -g3

#if Linux
ifeq ($(PLATFORM),linux)
INC += $(shell pkg-config --cflags sdl2)
LINKS = $(shell pkg-config --libs sdl2)
LINKS += -lGL -lGLU -lglut -lm -lXt -lX11 -LlibLinux -lqu3e
endif



all: $(OBJ_FILES_COMMON) $(OBJ_FILES_CPP)
	$(GCC) obj/*.o $(MAIN) -o $(OUT) $(INC) $(LINKS)

obj/%.o : src/common/%.c
	$(GCC) -c -o $@ $< $(INC)


obj/%.o : src/common/%.cpp
	$(GCC) -c -o $@ $< $(INC)

obj/%.o : src/%.cpp
	$(GCC) -c -o $@ $< $(INC)

clean :
	rm obj/*.o
