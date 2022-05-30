CC=gcc
CFLAGS=-I -Wall -g
DEPS = list.h stack.h graph.h io.h schedule.h
OBJ = detect.o list.o stack.o graph.o io.o schedule.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

detect: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f detect *.o *~ core $(INCDIR)/*~ 