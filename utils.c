#include "utils.h"

int obter_qntd_caixas(char* buffer) {
	char* token;

	strtok(buffer, "=");
	token = strtok(NULL, "=");

	return atoi(token);
}

int obter_tempo_por_oper(char* buffer) {
	char* token;

	strtok(buffer, "=");
	token = strtok(NULL, "=");

	return atoi(token);
}

int* obter_disc_escalonamento(char* buffer) {
	int i;
	int* tmp;
	char* token;

	tmp = (int*) malloc(sizeof(int) * 5);

	strtok(buffer, "=");
	strtok(NULL, "{");
	token = strtok(NULL, "}");

	i = 0;
	token = strtok(token, ",");
	while (token != NULL) {
		tmp[i] = atoi(token);
		token = strtok(NULL, ",");
		i++;
	}

	return tmp;
}

Cliente* obter_info_cliente(char* buffer) {
	Cliente *cliente;
	char* token_01;
	char* token_02;
	char* token_03;

	cliente = (Cliente*) malloc(sizeof(Cliente));
	token_01 = strtok(buffer, "-");
	token_02 = strtok(NULL, "-");
	token_03 = strtok(NULL, "-");

	cliente->classe = (char*) malloc(sizeof(char) * (strlen(token_01) + 1));
	strcpy(cliente->classe, token_01);

	strtok(token_02, " ");
	cliente->num_conta = atoi(strtok(NULL, " "));

	cliente->qntd_opereracoes = atoi(strtok(token_03, " "));

	return cliente;
}

int obter_classe_num(char* classe) {
	if (strcmp("Premium", classe) == 0) {
		return 1;
	} else if (strcmp("Ouro", classe) == 0) {
		return 2;
	} else if (strcmp("Prata", classe) == 0) {
		return 3;
	} else if (strcmp("Bronze", classe) == 0) {
		return 4;
	} else {
		return 5;
	}
}