# Makefile for Network Routing Simulator

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = network_simulator
SRCS = main.c network_functions.c
HEADERS = network_types.h network_functions.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
