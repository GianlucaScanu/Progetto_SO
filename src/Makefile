CC=gcc

CFLAGS=-I. -lm

DEPS = allocator.h bit_map.h tree.h

OBJ = main.o allocator.o bit_map.o tree.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
