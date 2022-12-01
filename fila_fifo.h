#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _no {
  int valor;
  int chave;
  struct _no* prox; 
} No;

typedef struct {
  No* primeiro;
  No* ultimo;
  int tamanho;
} FILA_FIFO;

void f_inicializar(FILA_FIFO **f);
int f_inserir(FILA_FIFO **f, int chave, int valor);
void dequeue(FILA_FIFO **f);
int f_obter_proxima_chave (FILA_FIFO **f);
int f_consultar_proxima_chave(FILA_FIFO **f);
int f_consultar_proximo_valor(FILA_FIFO **f);
int f_num_elementos (FILA_FIFO **f);
int f_consultar_chave_por_posicao (FILA_FIFO **f, int posicao);
int f_consultar_valor_por_posicao (FILA_FIFO **f, int posicao);

#endif /* QUEUE_H */