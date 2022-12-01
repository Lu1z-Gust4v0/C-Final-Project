main: fila_filo.o escalonador.o main.c
	gcc fila_filo.o escalonador.o main.c -o main
escalonador.o: escalonador.c
	gcc -c escalonador.c
fila_filo.o: fila_filo.c
	gcc -c fila_filo.c
clean: 
	rm -rf *.o