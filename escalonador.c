#include "escalonador.h"

void e_inicializar(Escalonador *e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5) {
  int i;

  e->caixas = caixas;
  e->tempo_por_operacao = delta_t;
  e->iteracao = 0;

  e->ordem_de_chamada[0] = n_1;
  e->ordem_de_chamada[1] = n_2; 
  e->ordem_de_chamada[2] = n_3;
  e->ordem_de_chamada[3] = n_4;
  e->ordem_de_chamada[4] = n_5;

  for (i = 0; i < 5; i++) {
    // Allocate memory for each queue.
    f_inicializar(&e->filas[i]);
  }
}

int e_inserir_por_fila(Escalonador *e, int classe, int num_conta, int qtde_operacoes) {
  int code;

  code = f_inserir(&e->filas[classe - 1], num_conta, qtde_operacoes);
  
  return code;
}

int e_consultar_prox_fila(Escalonador *e) {
  int threshold_01 = e->ordem_de_chamada[0]; 
  int threshold_02 = e->ordem_de_chamada[1] + threshold_01;
  int threshold_03 = e->ordem_de_chamada[2] + threshold_02;
  int threshold_04 = e->ordem_de_chamada[3] + threshold_03; 
  int threshold_05 = e->ordem_de_chamada[4] + threshold_04;

  int ordem_de_chamada = e->iteracao % threshold_05; 

  // Queue 01;
  if (ordem_de_chamada < threshold_01) {
    return 1;
  // Queue 02;
  } else if (ordem_de_chamada < threshold_02) {
    return 2;
  // Queue 03;
  } else if (ordem_de_chamada < threshold_03) {
    return 3;
  // Queue 04;
  } else if (ordem_de_chamada < threshold_04) {
    return 4;
  // Queue 05;
  } else {
    return 5;
  }
}

int e_consultar_prox_num_conta(Escalonador *e) {
  int num_fila;

  num_fila = e_consultar_prox_fila(e);

  return f_consultar_proxima_chave(&e->filas[num_fila - 1]);
}

int e_consultar_prox_qtde_oper(Escalonador *e) {
  int num_fila;

  num_fila = e_consultar_prox_fila(e);

  return f_consultar_proximo_valor(&e->filas[num_fila - 1]);
}

int e_consultar_qtde_clientes(Escalonador *e) {
  int i, qntd;

  for (i = 0; i < 5; i++) {
    qntd += f_num_elementos(&e->filas[i]);
  }

  return qntd;
}

int e_consultar_tempo_prox_cliente(Escalonador *e) {
  int qntd_operacoes;

  qntd_operacoes = e_consultar_prox_qtde_oper(e);

  return qntd_operacoes * e->tempo_por_operacao;
}

int e_obter_prox_num_conta(Escalonador *e) {
  int conta, num_fila;

  num_fila = e_consultar_prox_fila(e);
  conta = f_obter_proxima_chave(&e->filas[num_fila - 1]);

  // Increment scheduler iteration
  e->iteracao++;

  return conta;
}