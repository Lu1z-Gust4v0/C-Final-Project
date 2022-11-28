main: queue.o main.c
	gcc queue.o main.c -o main
queue.o: queue.c
	gcc -c queue.c
clean: 
	rm -rf *.o