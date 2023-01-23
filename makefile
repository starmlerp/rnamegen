CPPFLAGS=-g3 -lm# -fsanitize=address -Wall
CC=gcc
CXX=g++
TARGET=main
SRC=./src
LIB=./lib
HDR=./include
BLD=./build

SRCS=$(wildcard $(SRC)/*.cpp)
HDRS=$(wildcard $(HDR)/*)
OBJS=$(SRCS:$(SRC)/%.cpp=$(BLD)/%.o)
LIBS=$(SRCS:$(SRC)/%.cpp=$(LIB)/lib%.a)

all: $(TARGET) tags $(LIB) $(BLD)

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CPPFLAGS) -c -I $(HDR) $(TARGET).cpp -o $@


$(TARGET): $(TARGET).o $(LIBS)
	$(CXX) $(TARGET).o -L$(LIB) $(OBJS:$(BLD)/%.o=-l%) $(CPPFLAGS) -o $@

$(LIB) $(BLD):
	mkdir $@

$(OBJS): $(BLD)/%.o: $(SRC)/%.cpp $(HDR)/%.h
	$(CXX) -c -I $(HDR) $(CPPFLAGS) $(word 1, $^) -o $@

$(LIBS): $(LIB)/lib%.a: $(BLD)/%.o
	ar rcs $@ $^

tags: $(TARGET).cpp $(HDRS) $(SRCS)
	ctags $^

.PHONY=run clear

run: $(TARGET)
	./$(TARGET)
clear:
	rm $(TARGET) $(TARGET).o $(OBJS) $(LIBS)
