CXX := g++

BUILD_DIR ?= ./build

INC_FLAGS := -I..
CXXFLAGS := $(INC_FLAGS) -Ofast -std=c++17

.PHONY: test

test: $(BUILD_DIR)/arrayutilstest.out $(BUILD_DIR)/boundingboxtest.out $(BUILD_DIR)/mathutilstest.out $(BUILD_DIR)/fluidutilstest.out

$(BUILD_DIR)/arrayutilstest.out: $(TEST_DIR)/arrayutilstest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/boundingboxtest.out: $(TEST_DIR)/boundingboxtest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/mathutilstest.out: $(TEST_DIR)/mathutilstest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

$(BUILD_DIR)/fluidutilstest.out: $(TEST_DIR)/fluidutilstest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@
