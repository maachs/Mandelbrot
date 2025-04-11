all: mandelbrot

BUILD_DIR = build
SRC = src
INCLUDE = include

mandelbrot: $(SRC)/Ticks.cpp $(SRC)/main.cpp $(SRC)/SimpleVersion.cpp $(SRC)/ArrayVersion.cpp $(SRC)/IntrinVersion.cpp $(INCLUDE)/Mandelbrot.h
	g++ -I $(INCLUDE)/ $(SRC)/main.cpp $(SRC)/Ticks.cpp $(SRC)/ArrayVersion.cpp $(SRC)/SimpleVersion.cpp $(SRC)/IntrinVersion.cpp -o $(BUILD_DIR)/mandelbrot.out -O3 -lsfml-graphics -lsfml-window -lsfml-system



