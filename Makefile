CXXFLAGS=-std=c++14 -Wall -Wextra -pedantic -g3  -l$(LIB_NAME) -l$(LIB_REGEX) -I include 
CXX=g++
#LIB_PATH=/usr/local/Cellar/boost/1.64.0_1/lib
LIB_NAME=boost_program_options
LIB_REGEX=boost_regex
SRC= main.cpp Net.cpp Neuron.cpp Reader.cpp
BIN= Net 

VPATH=src

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
clean: 
	@rm -rf $(BIN)
