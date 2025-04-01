all: simple

#simple: SimpleVersion.o
#	g++ SimpleVersion.o -lsfml-graphics -lsfml-window -lsfml-system

simple:
	 g++ SimpleVersion.cpp -o SimpleVersion.out -O3 -lsfml-graphics -lsfml-window -lsfml-system
