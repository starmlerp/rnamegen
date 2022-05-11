CPPFLAGS= -g

CC=gcc

TARGET=rnamegen
CTYPE=c

$(TARGET): $(TARGET).$(CTYPE)
	$(CC) $(CPPFLAGS) $^ -o $@

.PHONY=run

run: $(TARGET)
	./$(TARGET)
