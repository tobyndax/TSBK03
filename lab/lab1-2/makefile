ifeq ($(shell uname), Darwin)
  PLATFORM=mac
endif

ifeq ($(shell uname), Linux)
  PLATFORM=linux
endif
ifeq ($(PLATFORM),mac)


#Mac frameworks
LDFLAGS = -framework GLUT -framework OpenGL -framework Cocoa
MICRO = ../common/mac/MicroGlut.m

endif

ifeq ($(PLATFORM),linux)
LDFLAGS = -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11
MICRO = ../common/MicroGlut.c
endif


#Compiler flags, enable warnings and c99 standard
GCCFLAGS = -Wall -std=c99 -I../common
#Binaries that are needed for all sub-compilations
BINARIES = ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c $(MICRO)


all: lab0
#target: dependencies
#’tab’ 	compilation commands


lab0: lab1-2.c $(BINARIES)
	gcc $(GCCFLAGS) $(BINARIES) lab1-2.c -o lab1-2 $(LDFLAGS)


#compile MicroGlut on it’s own and ignore warnings.
MicroGlut.o: $(MICRO)
	gcc -w -c $(MICRO)

clean:
	rm lab1-1

system:
	printf $(shell uname)
