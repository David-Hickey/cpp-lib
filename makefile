CXX := g++

TEST_DIR ?= ./tests
BUILD_DIR ?= $(TEST_DIR)/build
SRC_DIR ?= .


INC_FLAGS := -I.
CXXFLAGS := $(INC_FLAGS) -O3 -std=c++17

.PHONY: test all clean

all:

test: $(BUILD_DIR)/arrayutilstest.out $(BUILD_DIR)/tensorutilstest.out $(BUILD_DIR)/boundingboxtest.out $(BUILD_DIR)/mathutilstest.out $(BUILD_DIR)/fluidutilstest.out $(BUILD_DIR)/randomutilstest.out

$(BUILD_DIR)/arrayutilstest.out: $(TEST_DIR)/arrayutilstest.cpp $(SRC_DIR)/arrayutils.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/tensorutilstest.out: $(TEST_DIR)/tensorutilstest.cpp $(SRC_DIR)/tensorutils.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/boundingboxtest.out: $(TEST_DIR)/boundingboxtest.cpp $(SRC_DIR)/boundingbox.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/mathutilstest.out: $(TEST_DIR)/mathutilstest.cpp $(SRC_DIR)/mathutils.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/fluidutilstest.out: $(TEST_DIR)/fluidutilstest.cpp $(SRC_DIR)/fluidutils.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/randomutilstest.out: $(TEST_DIR)/randomutilstest.cpp $(SRC_DIR)/randomutils.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

clean:
	rm -rf $(BUILD_DIR)/*
