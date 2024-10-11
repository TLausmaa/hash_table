CC = gcc
CFLAGS = -Wall

SRC_DIR = ./src
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/hash.c 
OBJ = $(SRC:.c=.o)
TARGET = hash_table

all: $(TARGET) cleanobjects

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET)

cleanobjects:
	rm -f $(OBJ)

run: $(TARGET) cleanobjects runb

runb:
	./$(TARGET)
