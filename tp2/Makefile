#### PROJECT SETTINGS ####
BIN_NAME := main
CXX ?= g++
SRC_EXT = cpp
# LIBS = -lchilkat-9.5.0 -lresolv -lpthread
FLAGS = -std=c++14 -Wall -Wextra -g -fpermissive
### END PROJECT SETTINGS ####

export LD_LIBRARY_PATH="/usr/local/lib"

all:
	@$(CXX) *.$(SRC_EXT) -o $(BIN_NAME) $(FLAGS) $(shell pkg-config --cflags --libs gumbo)

# run this program
run:
	@-rm -rf terms/*
	@./$(BIN_NAME) ri_2018_collection.dat

test:
	@./$(BIN_NAME) html_pages.txt

clean:
	@$(RM) $(BIN_NAME)
	@$(RM) *.index *.urllist terms/* hash_table.tbl terms.dump
