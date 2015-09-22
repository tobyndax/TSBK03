ifeq ($(shell uname), Darwin)
  PLATFORM=mac
endif

ifeq ($(shell uname), Linux)
  PLATFORM=linux
endif
ifeq ($(PLATFORM),mac)
#Mac frameworks
LDFLAGS = -framework GLUT -framework OpenGL -framework Cocoa
MICRO = ../common/Mac/MicroGlut.o

endif

ifeq ($(PLATFORM),linux)
LDFLAGS = -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11
MICRO = ../common/MicroGlut.c
endif


#Compiler flags, enable warnings and c99 standard
GCCFLAGS = -Wall -std=c99 -I../common
#Binaries that are needed for all sub-compilations
BINARIES = ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c $(MICRO)

MAIN = lab3.c


all: lab3
#target: dependencies
#’tab’ 	compilation commands

lab3: $(BINARIES)
	gcc $(GCCFLAGS) $(BINARIES) $(MAIN) -o a.out $(LDFLAGS)

#compile MicroGlut on it’s own and ignore warnings.
MicroGlut.o: $(MICRO)
	gcc -w -c $(MICRO)

clean:
	rm -f *.o
	rm -f a.out

system:
	printf $(shell uname)
