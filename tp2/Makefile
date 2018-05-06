#### PROJECT SETTINGS ####
BIN_NAME := main
CXX ?= g++
SRC_EXT = cpp
# LIBS = -lchilkat-9.5.0 -lresolv -lpthread
FLAGS = -std=c++14 -Wall -Wextra -g -fpermissive
#### END PROJECT SETTINGS ####

export LD_LIBRARY_PATH="/usr/local/lib"

all:
	@$(CXX) *.$(SRC_EXT) -o $(BIN_NAME) $(FLAGS) $(shell pkg-config --cflags --libs gumbo)

# run this program
run:
	@./$(BIN_NAME)

clean:
	@$(RM) $(BIN_NAME)
