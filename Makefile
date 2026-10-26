CC = g++
EXEC = test
CXXFLAGS = -std=c++17 -Wpedantic -Wall -Wextra -Werror -g -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -lGL -lglfw -O2
BUILD_DIR = build
SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.cpp)
FILENAMES := $(notdir $(SRC))
OBJ := $(FILENAMES:%.cpp=$(BUILD_DIR)/%.o)

build-test: | $(BUILD_DIR) $(OBJ)
	@$(CC) $(CXXFLAGS) $(OBJ) -o $(BUILD_DIR)/$(EXEC)

run: build-test
	@$(BUILD_DIR)/$(EXEC)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@$(CC) $(CXXFLAGS) -c $^ -o $@

clean:
	@rm -rf $(BUILD_DIR)