main: fila_fifo.o logtree.o escalonador.o utils.o output.o main.c
	gcc fila_fifo.o logtree.o escalonador.o utils.o output.o main.c -o main
escalonador.o: escalonador.c
	gcc -c escalonador.c
fila_fifo.o: fila_fifo.c
	gcc -c fila_fifo.c
logtree.o: logtree.c
	gcc -c logtree.c
utils.o: utils.c
	gcc -c utils.c
output.o: output.c
	gcc -c output.c
test: fila_fifo.o logtree.o escalonador.o utils.o output.o main.c
	gcc fila_fifo.o logtree.o escalonador.o utils.o output.o main.c -o ./tests/main
clean: 
	rm -rf *.o main ./tests/main