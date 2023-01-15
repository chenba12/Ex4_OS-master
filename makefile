CC = gcc
CFLAGS = -Wall -g
all: graph
graph: main.c graph.h algo.c
	$(CC) $(CFLAGS) main.c algo.c graph.h -o graph -lm
PartB:betterping.c
	$(CC) $(CFLAGS) betterping.c -o PartB
clean:
	rm -f *.o graph
