FLAGS = -Wall -Wno-unused-result

DEPS = forward_list.h hash.h
OBJ = forward_list.c hash.c main.c

%.o: %.c $(DEPS)
	gcc -g -c -o $@ $< $(FLAGS)

all: $(OBJ)
	gcc -g -o main $(OBJ) $(FLAGS)

clean:
	rm -f main *.o

run: 
	./main	