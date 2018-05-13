#### PROJECT SETTINGS ####
BIN_NAME_PASS1 := pass1.out
BIN_NAME_PASS2 := pass2.out
CXX ?= g++
SRC_EXT = cpp
# LIBS = -lchilkat-9.5.0 -lresolv -lpthread
FLAGS = -std=c++14 -Wall -Wextra -g -fpermissive
### END PROJECT SETTINGS ####

export LD_LIBRARY_PATH="/usr/local/lib"

all:
#	# @$(CXX) pass1/*.$(SRC_EXT) -o $(BIN_NAME_PASS1) $(FLAGS) $(shell pkg-config --cflags --libs gumbo)
	@$(CXX) pass2/*.$(SRC_EXT) -o $(BIN_NAME_PASS2) $(FLAGS)

# run this program
run:
	# @-rm -rf terms/*
	# @./$(BIN_NAME_PASS1) ri_2018_collection.dat

test:
#	# @-rm -rf terms/*
#	# @./$(BIN_NAME_PASS1) html_pages.txt
	@./$(BIN_NAME_PASS2) html_pages.txt

clean:
	# @$(RM) $(BIN_NAME_PASS1) $(BIN_NAME_PASS2)
	# @$(RM) *.index *.urllist terms/* hash_table.tbl terms.dump
