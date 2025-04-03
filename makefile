all: simple && simple-O && array && array-O

simple: ./SimpleVersion/SimpleVersion.cpp ./SimpleVersion/SimpleVersion.h
	g++ ./SimpleVersion/SimpleVersion.cpp -o SimpleVersion.out -lsfml-graphics -lsfml-window -lsfml-system

simple-O: ./SimpleVersion/SimpleVersion.cpp ./SimpleVersion/SimpleVersion.h
	g++ ./SimpleVersion/SimpleVersion.cpp -o SimpleVersion.out -O3 -lsfml-graphics -lsfml-window -lsfml-system

array-O: ./ArrayVersion/ArrayVersion.cpp ./ArrayVersion/ArrayVersion.h
	g++ ./ArrayVersion/ArrayVersion.cpp -o ArrayVersion.out -O3 -lsfml-graphics -lsfml-window -lsfml-system

array: ./ArrayVersion/ArrayVersion.cpp ./ArrayVersion/ArrayVersion.h
	g++ ./ArrayVersion/ArrayVersion.cpp -o ArrayVersion.out -lsfml-graphics -lsfml-window -lsfml-system
