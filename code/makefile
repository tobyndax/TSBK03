ifeq ($(shell uname), Darwin)
  PLATFORM=mac
endif

ifeq ($(shell uname), Linux)
  PLATFORM=linux
endif
ifeq ($(PLATFORM),mac)


#Mac frameworks
LDFLAGS = -framework GLUT -framework OpenGL -framework Cocoa
MICRO = Mac/MicroGlut.m

endif

ifeq ($(PLATFORM),linux)
LDFLAGS = -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11
MICRO = MicroGlut.c
endif


#Compiler flags, enable warnings and c99 standard
GCCFLAGS = -Wall -std=c99
#Binaries that are needed for all sub-compilations
BINARIES = GL_utilities.c loadobj.c VectorUtils3.c LoadTGA.c MicroGlut.o globals.c terrain.c


all: orion
#target: dependencies
#’tab’ 	compilation commands


orion: projectOrion.c $(BINARIES)
	gcc $(GCCFLAGS) $(BINARIES) projectOrion.c -o projectOrion $(LDFLAGS)


#compile MicroGlut on it’s own and ignore warnings.
MicroGlut.o: $(MICRO)
	gcc -w -c $(MICRO)

clean:
	rm projectOrion
	rm *.o

system:
	printf $(shell uname)
