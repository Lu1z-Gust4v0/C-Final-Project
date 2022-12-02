#include <stdio.h>
#include <stdlib.h>
#include "escalonador.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  	FILE* file;

	char buffer[100];
	int qntd_caixas, delta_t, line_count;
	int* disciplina;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("[Error] Could not open file.\n");
		return 1;
	}

	while(fgets(buffer, 100, file)) {
		if (line_count == 0) {
			qntd_caixas = obter_qntd_caixas(buffer);
		} else if (line_count == 1) {
			delta_t = obter_tempo_por_oper(buffer);
		} else if (line_count == 2) {
			disciplina = obter_disc_escalonamento(buffer);
		} else if (line_count == 3) {
			// e_inicializar(&e, qntd_caixas, delta_t, disciplina[0], disciplina[1], disciplina[2], disciplina[3], disciplina[4]);
		} 
		line_count++;
	}
	
	printf("qntd caixas: %i\n", qntd_caixas);
	printf("tempo por oper: %i\n", delta_t);
	printf("iteração: %i\n", 0);

	for (int i = 0; i < 5; i++) {
		printf("disciplina: %i\n", disciplina[i]);
	}

	fclose(file);
	return 0;
}