.PHONY: all clean
#make -j$(nproc)
CXX := g++
CXXFLAGS := -O3 -march=native -I ./include/ -flto
LDFLAGS := -O3 -flto -lsfml-graphics -lsfml-window -lsfml-system

BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

TARGET := $(BUILD_DIR)/mandelbrot.out

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/*.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o


