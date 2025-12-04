CC = gcc
# TODO: deberia tener optimización o no???
CFLAGS = -Wall -Wextra -O2 -Isource

SRC_DIR = source

# object files 
OBJ = main.o $(SRC_DIR)/pager.o

# nombre de output file
TARGET = sim

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# main.c
$main.o: main.c $(SRC_DIR)/pager.h
	$(CC) $(CFLAGS) -c main.c -o /main.o

# pager.c
$(SRC_DIR)/pager.o: $(SRC_DIR)/pager.c $(SRC_DIR)/pager.h $(SRC_DIR)/types.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/pager.c -o $(SRC_DIR)/pager.o

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
