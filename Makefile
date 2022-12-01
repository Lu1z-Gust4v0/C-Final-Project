main: fila_fifo.o escalonador.o utils.o main.c
	gcc fila_fifo.o escalonador.o utils.o main.c -o main
escalonador.o: escalonador.c
	gcc -c escalonador.c
fila_fifo.o: fila_fifo.c
	gcc -c fila_fifo.c
utils.o: utils.c
	gcc -c utils.c
clean: 
	rm -rf *.o