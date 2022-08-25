CPPFLAGS= -static-libgcc -lm -std=c11 -g -Wall -Wextra 
CC=gcc

TARGET=rnamegen
CTYPE=c

$(TARGET): $(TARGET).$(CTYPE)
	$(CC) $(CPPFLAGS) $^ -o $@

tags: $(TARGET).$(CTYPE)
	ctags $^

.PHONY=run

run: $(TARGET)
	./$(TARGET)
