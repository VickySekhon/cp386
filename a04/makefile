# Makefile

CC = gcc
CFLAGS = -std=gnu99 -Wall
TARGET = allocation

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

run: $(TARGET)
	./$(TARGET) 1048576

clean:
	rm -f $(TARGET)
