#include "logtree.h"

void log_inicializar(Log **l) {
	(*l) = (Log*) malloc(sizeof(Log));
	(*l)->raiz = NULL;
}

void inserir_folha(Log **arvore, TREE_NODE *folha) {
	TREE_NODE* ptr = (*arvore)->raiz;
	
	if (ptr == NULL) {
		(*arvore)->raiz = folha;
		return;
	}

	if (folha == NULL) return;
		
	while(ptr != folha) {
		if (folha->timer <= ptr->timer) {
			if (ptr->esquerda == NULL) {
				folha->pai = ptr;	
				ptr->esquerda = folha;			
			}
			ptr = ptr->esquerda;		
		} else {
			if (ptr->direita == NULL) {
				folha->pai = ptr;	
				ptr->direita = folha;				
			}
			ptr = ptr->direita;
		}
	}
}

TREE_NODE* criar_folha(int conta, int classe, int timer, int caixa, int ops) {
	TREE_NODE* folha;

	folha = (TREE_NODE*) malloc(sizeof(TREE_NODE));
	if (folha == NULL) {
		return NULL;
	}

	folha->conta = conta;
	folha->classe = classe;
	folha->timer = timer;
	folha->caixa = caixa;
	folha->operacoes = ops;

	folha->pai = NULL;
	folha->direita = NULL;
	folha->esquerda = NULL;

	return folha;
}

void log_registrar(Log **l, int conta, int classe, int timer, int caixa, int ops) {
	TREE_NODE* folha;

	folha = criar_folha(conta, classe, timer, caixa, ops);
	
	inserir_folha(l, folha);
}

int soma_por_classe(TREE_NODE *folha, int classe) {
	if (folha == NULL) return 0;
	
	return (
		(folha->classe == classe ? folha->timer : 0) +
		soma_por_classe(folha->direita, classe) +
		soma_por_classe(folha->esquerda, classe)
	);
}

int contagem_por_classe(TREE_NODE *folha, int classe) {
	if (folha == NULL) return 0;
	
	return (
		(folha->classe == classe ? 1 : 0) +
		contagem_por_classe(folha->direita, classe) +
		contagem_por_classe(folha->esquerda, classe)
	);
}

int operacoes_por_classe(TREE_NODE *folha, int classe) {
	if (folha == NULL) return 0;

	return (
		(folha->classe == classe ? folha->operacoes : 0) +
		operacoes_por_classe(folha->direita, classe) +
		operacoes_por_classe(folha->esquerda, classe) 
	);
}

int log_obter_soma_por_classe(Log **l, int classe) {
	TREE_NODE* raiz = (*l)->raiz; 
	
	if (raiz == NULL) return 0;

	return soma_por_classe(raiz, raiz->classe);
}

int log_obter_contagem_por_classe(Log **l, int classe) {
	TREE_NODE* raiz = (*l)->raiz; 

	if (raiz == NULL) return 0;

	return contagem_por_classe(raiz, raiz->classe);
}

float log_media_por_classe(Log **l, int classe) {
	return (
		log_obter_soma_por_classe(l, classe) /
		log_obter_contagem_por_classe(l, classe) 
	);
}


float log_media_ops_por_classe(Log **l, int classe) {
	TREE_NODE* raiz = (*l)->raiz; 

	if (raiz == NULL) return 0;
	
	return (
		operacoes_por_classe(raiz, classe) /
		log_obter_contagem_por_classe(l, classe) 
	);	
}