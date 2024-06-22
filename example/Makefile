CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OUTPUT = bin/a.out
SRC := $(shell find . ../src -name '*.c')
OBJ := $(SRC:.c=.o)

build: $(OUTPUT) clean_objs

$(OUTPUT): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./$(OUTPUT)

clean_objs:
	rm -f $(OBJ)

clean: clean_objs
	rm -f $(OUTPUT)
