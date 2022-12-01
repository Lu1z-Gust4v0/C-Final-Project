#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* classe;
	int num_conta;
	int qntd_opereracoes;
} Cliente;

int obter_qntd_caixas(char* buffer); 
int obter_tempo_por_oper(char* buffer);
int* obter_disc_escalonamento(char* buffer);
Cliente* obter_info_cliente(char* buffer);
int obter_classe_num(char* classe);

#endif /* UTILS_H */