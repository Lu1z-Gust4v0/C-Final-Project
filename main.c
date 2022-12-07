#include <stdio.h>
#include <stdlib.h>
#include "escalonador.h"

int main(int argc, char* argv[]) {
	Escalonador e; 

	if (argc != 3) {
		printf("Missing arguments.\n");
    printf("Correct: './main [arq_in] [arq_out]'\n");
		return EXIT_FAILURE;
	}

  e_rodar(&e, argv[1], argv[2]);

	return EXIT_SUCCESS;
}