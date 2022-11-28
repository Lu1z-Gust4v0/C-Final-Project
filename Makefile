main: fila_fifo.o main.c
	gcc fila_fifo.o main.c -o main
fila_fifo.o: fila_fifo.c
	gcc -c fila_fifo.c
clean: 
	rm -rf *.o