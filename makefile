CPPFLAGS= -static-libgcc -g -lm # -Wall -Wextra 
CC=gcc
CXX=g++
TARGET=main
SRC=./src
LIB=./lib
HDR=./include

LIBS=$(wildcard $(SRC)/*.cpp)
HDRS=$(wildcard $(HDR)/*)
OBJS=$(LIBS:$(SRC)/%.cpp=$(LIB)/%.o)

all: $(TARGET) tags

$(TARGET).o: $(TARGET).cpp $(OBJS)
	$(CXX) -c -I $(HDR) $(CPPFLAGS) $^ -o $@

$(TARGET): $(TARGET).o $(OBJS)
	$(CXX) $^ -o $@

$(LIB):
	mkdir $(LIB)

$(OBJS): $(LIBS) $(LIB)
	$(CXX) -c -I $(HDR) $(CPPFLAGS) $(LIBS) -o $@

tags: %.cpp $(HDR)/%.h
	ctags $^

.PHONY=run clear

run: $(TARGET)
	./$(TARGET)
clear:
	rm $(TARGET) $(TARGET).o $(OBJS) tags
