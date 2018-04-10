#### PROJECT SETTINGS ####
BIN_NAME := main
CXX ?= g++
SRC_EXT = cpp
# LIBS = -lchilkat-9.5.0 -lresolv -lpthread
FLAGS = -std=c++14 -Wall -Wextra -g -fpermissive -I ./chilkat/include/
#### END PROJECT SETTINGS ####

UNAME_S := $(shell uname -s)
TOP := $(shell pwd)
ifeq ($(UNAME_S),Linux)
	FLAGS += $(TOP)/chilkat/lib/libchilkat-9.5.0.so
endif
ifeq ($(UNAME_S),Darwin)
	FLAGS += chilkat/lib/libchilkat.a
endif

all:
	@$(CXX) *.$(SRC_EXT) -o $(BIN_NAME) $(FLAGS) -lpthread

# run this program
run:
	@./$(BIN_NAME)

clean:
	@$(RM) $(BIN_NAME)
	@$(RM) output_file.txt unique_id_file.txt


