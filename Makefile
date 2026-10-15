CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = HW6
SRC = HW6.c

.PHONY: main clean

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

