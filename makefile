CPPFLAGS=-g3 -lm# -fsanitize=address -Wall
CC=gcc
CXX=g++

TARGET=main

SRC=./src
LIB=./lib
HDR=./include
BLD=./build

INSTALL=/usr/bin/rnamegen

CONFIG=names.csv
CONFIGDIR=./data
INSTALLDIR=/etc/rnamegen

SRCS=$(wildcard $(SRC)/*.cpp)
HDRS=$(wildcard $(HDR)/*)
OBJS=$(SRCS:$(SRC)/%.cpp=$(BLD)/%.o)
LIBS=$(SRCS:$(SRC)/%.cpp=$(LIB)/lib%.a)

all: $(TARGET) tags $(LIB) $(BLD)
install: $(INSTALL)

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CPPFLAGS) -c -I $(HDR) $(TARGET).cpp -o $@

$(TARGET): $(TARGET).o $(LIBS)
	$(CXX) $(TARGET).o -L$(LIB) $(OBJS:$(BLD)/%.o=-l%) $(CPPFLAGS) -o $@
	chmod +x $@

$(LIB) $(BLD) $(INSTALLDIR):
	mkdir $@

$(OBJS): $(BLD)/%.o: $(SRC)/%.cpp $(HDR)/%.h
	$(CXX) -c -I $(HDR) $(CPPFLAGS) $(word 1, $^) -o $@

$(LIBS): $(LIB)/lib%.a: $(BLD)/%.o
	ar rcs $@ $^

$(INSTALLDIR)/$(CONFIG): $(CONFIGDIR)/$(CONFIG) $(INSTALLDIR)
	cp $(CONFIGDIR)/$(CONFIG) $@

tags: $(TARGET).cpp $(HDRS) $(SRCS)
	ctags $^

.PHONY=run clear uninstall

run: $(TARGET)
	./$(TARGET)
clear:
	rm -f $(TARGET) $(TARGET).o $(OBJS) $(LIBS)

$(INSTALL): $(TARGET) $(LIB) $(BLD) $(INSTALLDIR)/$(CONFIG)
	cp $(TARGET) $@

uninstall:
	rm -r $(INSTALLDIR) $(INSTALL)
