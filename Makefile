# Compiler setup.
CXX := g++
CXXFLAGS := $(INC_FLAGS) -std=c++20 -MMD -MP -g
LDFLAGS := -lncurses

# Build path & target.
TARGET_EXEC := snake
BUILD_DIR := ./bin
SRC_DIRS := ./src

# Source files.
SRCS := $(shell find $(SRC_DIRS) -name '*.cc')

# Object files.
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C++ source.
$(BUILD_DIR)/%.cc.o: %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/*