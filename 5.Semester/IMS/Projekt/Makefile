# @file:>-Makefile
# @author:>-Vojtěch Bůbela
# @login:>xbubel08
# @brief:>Makefile for IMS project

# compiler
CXX = g++
CC = $(CXX)

# dirs
SRC_DIR := ./src

# flags
CPP_FLAGS = -g -Wall -Werror

# source files
SRC_FILES = $(SRC_DIR)/*.cpp
HEADER_FILES = $(SRC_DIR)/*.hpp

# target file
TARGET = simulation

all: $(TARGET)

$(TARGET): $(SRC_FILES) #$(HEADER_FILES)
	$(CC) $(CPP_FLAGS) -o $(TARGET) $(SRC_FILES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)

