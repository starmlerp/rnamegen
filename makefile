CPPFLAGS=

CC=g++

TARGET=rnamegen
CTYPE=c

$(TARGET): $(TARGET).$(CTYPE)
	$(CC) $(CPPFLAGS) $^ -o $@

.PHONY=run

run: $(TARGET)
	./$(TARGET)
