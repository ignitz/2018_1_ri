#### PROJECT SETTINGS ####
BIN_NAME := main
CXX ?= g++
SRC_EXT = cpp
LIBS = -lchilkat-9.5.0 -lresolv -lpthread
COMPILE_FLAGS = -std=c++14 -Wall -Wextra -g -I ./include/
INCLUDES = include/
LINK_FLAGS = lib/
#### END PROJECT SETTINGS ####

all:
	@$(CXX) *.$(SRC_EXT) -o $(BIN_NAME) $(COMPILE_FLAGS) -I $(INCLUDES) -L $(LINK_FLAGS) $(LIBS)

# run this program
run:
	@./$(BIN_NAME)

clean:
	@$(RM) $(BIN_NAME)
