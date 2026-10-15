CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

SRC_DIR = src
OBJ_DIR = obj

# Source and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/Model.cpp
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/Model.o
TARGET = slm

# Default target builds the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for compiling main.cpp
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/Model.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for compiling Model.cpp
$(OBJ_DIR)/Model.o: $(SRC_DIR)/Model.cpp $(SRC_DIR)/Model.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target removes built objects and executable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean