# Compiler
CXX = g++

# Project name
TARGET = game

# Directories
SRC_DIR = src
INC_DIR = include

# Grab all .cpp files in src/
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Convert .cpp to .o (object files)
OBJ = $(SRC:.cpp=.o)

# Include flags
CXXFLAGS = -I$(INC_DIR) `sdl2-config --cflags`

# Linker flags (SDL2)
LDFLAGS = `sdl2-config --libs`

# Default target: build the game
all: $(TARGET)

# Link all object files to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile .cpp files to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the game
run: all
	./$(TARGET)

# Clean object files and executable
clean:
	rm -f $(OBJ) $(TARGET)
