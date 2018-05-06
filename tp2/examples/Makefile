#### PROJECT SETTINGS ####
CXX ?= g++
SRC_EXT = cc
# LIBS = -lchilkat-9.5.0 -lresolv -lpthread
FLAGS = -std=c++14 -Wall -Wextra -g -fpermissive
#### END PROJECT SETTINGS ####

export LD_LIBRARY_PATH="/usr/local/lib"

all:
	@$(CXX) clean_text.$(SRC_EXT) -o clean_text.out $(FLAGS) $(shell pkg-config --cflags --libs gumbo)
	@$(CXX) get_title.c -o get_title.out $(FLAGS) $(shell pkg-config --cflags --libs gumbo)
	@$(CXX) positions_of_class.$(SRC_EXT) -o positions_of_class.out $(FLAGS) $(shell pkg-config --cflags --libs gumbo)
	@$(CXX) prettyprint.$(SRC_EXT) -o prettyprint.out $(FLAGS) $(shell pkg-config --cflags --libs gumbo)
	@$(CXX) serialize.$(SRC_EXT) -o serialize.out $(FLAGS) $(shell pkg-config --cflags --libs gumbo)

clean:
	@$(RM) *.out
