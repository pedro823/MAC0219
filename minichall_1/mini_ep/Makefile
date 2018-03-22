# Breno Helfstein Moura       NUSP: 9790972
# Pedro Pereira               NUSP: -------

FLAGS = -pthread -Wall -pedantic -g
OBJS = main.o frog.o
EXEC = main

.PHONY: clean

all: run

run: $(OBJS)
	$(CC) $(FLAGS) $^ -o main
	make clean
	./main

%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	-rm -f *.o *~
