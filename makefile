all: mandelbrot #simple && simple-O && array && array-O

BUILD_DIR = build
SRC = src
INCLUDE = include

mandelbrot: $(SRC)/main.cpp $(SRC)/SimpleVersion.cpp $(SRC)/ArrayVersion.cpp $(SRC)/IntrinVersion.cpp $(INCLUDE)/Mandelbrot.h
	g++ -I $(INCLUDE)/ $(SRC)/main.cpp $(SRC)/ArrayVersion.cpp $(SRC)/SimpleVersion.cpp $(SRC)/IntrinVersion.cpp -o $(BUILD_DIR)/mandelbrot.out -lsfml-graphics -lsfml-window -lsfml-system



