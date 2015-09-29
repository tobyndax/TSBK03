ifeq ($(shell uname), Darwin)
  PLATFORM=mac
endif

ifeq ($(shell uname), Linux)
  PLATFORM=linux
endif
ifeq ($(PLATFORM),mac)


#Mac frameworks
LDFLAGS = -framework GLUT -framework OpenGL -framework Cocoa
MICRO = common/mac/MicroGlut.m

endif

ifeq ($(PLATFORM),linux)
LDFLAGS = -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11
MICRO = common/Linux/MicroGlut.c
endif


#Compiler flags, enable warnings and c99 standard
GCCFLAGS = -Wall -std=c99 -I../common
#Binaries that are needed for all sub-compilations
commondir = common/
BINARIES = $(commondir)LoadTGA.c SpriteLight.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(MICRO)


all: lab4
#target: dependencies
#’tab’ 	compilation commands

lab4: lab4.c $(BINARIES)
	gcc $(GCCFLAGS) $(BINARIES) lab4.c -o a.out $(LDFLAGS)


#compile MicroGlut on it’s own and ignore warnings.
MicroGlut.o: $(MICRO)
	gcc -w -c $(MICRO)

clean:
	rm a.out

system:
	printf $(shell uname)
