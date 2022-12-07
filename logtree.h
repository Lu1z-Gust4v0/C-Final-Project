#ifndef LOG_TREE_H
#define LOG_TREE_H

#include "stdio.h"
#include "stdlib.h"

typedef struct tree_node {
	struct tree_node* pai; 
	struct tree_node* direita; 
	struct tree_node* esquerda; 
	int conta; 
	int classe;
	int caixa; 
	int timer;
	int operacoes;
} TREE_NODE;

typedef struct {
	TREE_NODE *raiz;
} Log;

void log_inicializar(Log **l);
void log_registrar(Log **l, int conta, int classe, int timer, int caixa, int ops);
float log_media_por_classe(Log **l, int classe);
float log_media_ops_por_classe(Log **l, int classe);
int log_obter_soma_por_classe(Log **l, int classe);
int log_obter_contagem_por_classe(Log **l, int classe);

void log_free(Log **l);

#endif /* LOG_TREE_H */