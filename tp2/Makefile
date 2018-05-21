#### PROJECT SETTINGS ####
BIN_NAME_PASS1 := pass1.out
BIN_NAME_PASS2 := pass2.out
BIN_NAME_PASS3 := pass3.out
CXX ?= g++
SRC_EXT = cpp
# LIBS = -lchilkat-9.5.0 -lresolv -lpthread
FLAGS = -std=c++14 -Wall -Wextra -g -fpermissive
FILE_LIST = *.index *.urllist terms/* hash_table.tbl terms.dump
### END PROJECT SETTINGS ####

export LD_LIBRARY_PATH="/usr/local/lib"

all:
	@$(CXX) pass1/*.$(SRC_EXT) -o $(BIN_NAME_PASS1) $(FLAGS) $(shell pkg-config --cflags --libs gumbo)
	@$(CXX) pass2/*.$(SRC_EXT) -o $(BIN_NAME_PASS2) $(FLAGS)
	@$(CXX) pass3/*.$(SRC_EXT) -o $(BIN_NAME_PASS3) $(FLAGS)

# run this program
run:
	@-$(RM) $(FILE_LIST)
	@echo "\n**************************************\nInit PASS 1\n"
	@./$(BIN_NAME_PASS1) ri_2018_collection.dat
	@echo "\n**************************************\nInit PASS 2\n"
	@./$(BIN_NAME_PASS2) ri_2018_collection.dat
	@echo "\n**************************************\nInit PASS 3\n"
	@./$(BIN_NAME_PASS3) ri_2018_collection.dat

test:
	@-$(RM) $(FILE_LIST)
	@echo "\n**************************************\nInit PASS 1\n"
	@./$(BIN_NAME_PASS1) html_pages.txt
	@echo "\n**************************************\nInit PASS 2\n"
	@./$(BIN_NAME_PASS2) html_pages.txt
	@echo "\n**************************************\nInit PASS 3\n"
	@./$(BIN_NAME_PASS3) html_pages.txt

clean:
	@-$(RM) $(BIN_NAME_PASS1) $(BIN_NAME_PASS2) $(BIN_NAME_PASS3)
	@-$(RM) $(FILE_LIST)
