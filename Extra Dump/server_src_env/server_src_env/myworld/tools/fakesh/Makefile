CC=gcc
C_ARGS=-Wall -Wextra -pipe -g -c

fakesh : main.o cmd_line.o
	$(CC) main.o cmd_line.o -o fakesh

main.o : main.c cmd_line.h
	$(CC) $(C_ARGS) main.c

cmd_line.o : cmd_line.c cmd_line.h
	$(CC) $(C_ARGS) cmd_line.c

clean:
	rm *.o
	rm fakesh
