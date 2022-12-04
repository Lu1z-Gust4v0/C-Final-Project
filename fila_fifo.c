#include "fila_fifo.h"

void f_inicializar(FILA_FIFO **f) {
  *f = (FILA_FIFO*) malloc(sizeof(FILA_FIFO));  
  (*f)->primeiro = NULL;
  (*f)->ultimo = NULL;
  (*f)->tamanho = 0;
}

int f_num_elementos(FILA_FIFO **f) {
  return (*f)->tamanho;
}

int is_duplicated_key(FILA_FIFO **f, int chave) {
  No* ptr;

  for (ptr = (*f)->primeiro; ptr != NULL; ptr = ptr->prox) {
    if (ptr->chave == chave) {
      return 1;
    }
  }
  return 0;
}

int f_inserir(FILA_FIFO **f, int chave, int valor) {
  No *novo_no;
  // The queue was not initialized.
  if (*f == NULL) return 0;
  // Check if the provided key is duplicated
  if (is_duplicated_key(f, chave)) return 0;
  // Allocate memory to a new node.
  novo_no = (No*) malloc(sizeof(No));
  if (novo_no == NULL) {
    printf("Failed to allocate memory for a new node.\n");
    return 0;
  }

  novo_no->chave = chave;
  novo_no->valor = valor;
  novo_no->prox = NULL;

  // Case 1: There are no elements in the queue.
  if(f_num_elementos(f) == 0) {
    (*f)->primeiro = novo_no;
  // Case 2: There are elements in the queue. 
  } else {
    (*f)->ultimo->prox = novo_no;
  }  
  (*f)->ultimo = novo_no;
  (*f)->tamanho++;;

  return 1;
}

void dequeue(FILA_FIFO **f) {
  No* no;
  no = (*f)->primeiro;
  // Case 1: There is exactly one element in the queue.
  if (f_num_elementos(f) == 1) {
    (*f)->primeiro = NULL;
    (*f)->ultimo = NULL;
  // Case 2: There is more than one element in the queue.
  } else if (f_num_elementos(f) > 1) {
    (*f)->primeiro = no->prox;
  }
  (*f)->tamanho--;
  // Free alocated memory for the node.
  free(no);
}

int f_obter_proxima_chave (FILA_FIFO **f) {
  int chave;
  // The queue is not inilitialized or it's empty.
  if (*f == NULL || f_num_elementos(f) == 0) return -1;
  
  chave = (*f)->primeiro->chave;
  // Remove node from the queue.
  dequeue(f);

  return chave;
}

int f_consultar_proxima_chave(FILA_FIFO **f) {
  if ((*f)->primeiro == NULL) return 0;
  return (*f)->primeiro->chave;
}

int f_consultar_proximo_valor(FILA_FIFO **f) {
  if ((*f)->primeiro == NULL) return 0;
  return (*f)->primeiro->valor;
}

No* get_nth_node(FILA_FIFO **f, int posicao) {
  No *ptr;
  int counter = 0;

  ptr = (*f)->primeiro;
  // Since position starts at 1, we have to add 1 to the counter.
  while (counter + 1 < posicao) {
    ptr = ptr->prox;
    counter++;
  }
  return ptr;
}

int f_consultar_chave_por_posicao (FILA_FIFO **f, int posicao) {
  No *no;
  // The queue is not inilitialized or position provided exceeds queue length.
  if (*f == NULL || f_num_elementos(f) < posicao) return -1;

  no = get_nth_node(f, posicao);

  return no->chave;
}

int f_consultar_valor_por_posicao (FILA_FIFO **f, int posicao) {
  No *no;
  // The queue is not inilitialized or position provided exceeds queue length.
  if (*f == NULL || f_num_elementos(f) < posicao) return -1;

  no = get_nth_node(f, posicao);

  return no->valor;
}