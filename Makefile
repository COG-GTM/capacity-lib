CXX ?= g++
CC ?= gcc
CXXFLAGS ?= -O2 -Wall -Wextra
CFLAGS ?= -O2 -Wall -Wextra
BUILD = build
INCLUDES = -Iinclude

.PHONY: all lib check clean

all: lib $(BUILD)/test_capacity $(BUILD)/test_c_api

lib: $(BUILD)/libcapacity.a $(BUILD)/libcapacity.so

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/capacity.o: src/capacity.cpp include/capacity_lib/capacity.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -fPIC $(INCLUDES) -c -o $@ $<

$(BUILD)/libcapacity.a: $(BUILD)/capacity.o
	ar rcs $@ $^

$(BUILD)/libcapacity.so: $(BUILD)/capacity.o
	$(CXX) -shared -o $@ $^

$(BUILD)/test_capacity: tests/test_capacity.cpp $(BUILD)/libcapacity.a
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(BUILD)/libcapacity.a

$(BUILD)/test_c_api: tests/test_c_api.c $(BUILD)/libcapacity.a
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILD)/test_c_api.o $<
	$(CXX) -o $@ $(BUILD)/test_c_api.o $(BUILD)/libcapacity.a

check: $(BUILD)/test_capacity $(BUILD)/test_c_api
	$(BUILD)/test_capacity
	$(BUILD)/test_c_api

clean:
	rm -rf $(BUILD)
