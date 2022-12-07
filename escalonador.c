#include "escalonador.h"
#include "output.h"

void e_inicializar(Escalonador *e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5) {
  int i;

  e->qntd_caixas = caixas;
  e->tempo_por_operacao = delta_t;
  e->iteracao = 0;
  e->fila_atual = 0;

  e->ordem_de_chamada[0] = n_1;
  e->ordem_de_chamada[1] = n_2; 
  e->ordem_de_chamada[2] = n_3;
  e->ordem_de_chamada[3] = n_4;
  e->ordem_de_chamada[4] = n_5;

  e->caixas = (Caixa*) malloc(sizeof(Caixa) * caixas);
  if (e->caixas == NULL) {
    printf("[Error] Failed to allocate memory for 'cashiers'.\n");
    return;
  }

  // Allocate memory for each queue.
  for (i = 0; i < 5; i++) {
    f_inicializar(&e->filas[i]);
  }

  for (i = 0; i < caixas; i++) {
    e->caixas[i].atendimentos = 0;
    e->caixas[i].timer = 0;
  }
}

int e_inserir_por_fila(Escalonador *e, int classe, int num_conta, int qtde_operacoes) {
  return f_inserir(&e->filas[classe - 1], num_conta, qtde_operacoes);
}

int e_consultar_prox_fila(Escalonador *e) {
  int chamada = e->ordem_de_chamada[e->fila_atual];  
  int qntd = f_num_elementos(&e->filas[e->fila_atual]);

  if (e->iteracao == chamada || qntd == 0) {
    return (e->fila_atual + 1) % 5;
  } 

  return e->fila_atual;
}

int e_obter_prox_num_conta(Escalonador *e) {
  int conta, fila_anterior, counter;

  fila_anterior = e->fila_atual;
  counter = 0;

  do {
    e->fila_atual = e_consultar_prox_fila(e);
  } while (f_num_elementos(&e->filas[e->fila_atual]) == 0 && ++counter < 5);

  if (fila_anterior != e->fila_atual) {
    e->iteracao = 0;
  }

  conta = f_obter_proxima_chave(&e->filas[e->fila_atual]);
  // Increment scheduler iteration
  if (conta != -1) e->iteracao++;
  
  return conta;
}

int e_consultar_prox_num_conta(Escalonador *e) {
  int num_fila, num_conta, counter;

  num_fila = e_consultar_prox_fila(e);

  counter = 0;
  do {
    num_conta = f_consultar_proxima_chave(&e->filas[num_fila]);
    
    if (num_conta == 0) num_fila = (num_fila + 1) % 5;
    
  } while (num_conta == 0 && ++counter < 5); 

  return num_conta;
}

int e_consultar_prox_qtde_oper(Escalonador *e) {
  int num_fila, qntd_ops, counter;

  num_fila = e_consultar_prox_fila(e);

  counter = 0;
  do {
    qntd_ops = f_consultar_proximo_valor(&e->filas[num_fila]);
    
    if (qntd_ops == 0) num_fila = (num_fila + 1) % 5;
    
  } while (qntd_ops == 0 && ++counter < 5); 

  return qntd_ops;
}

int e_consultar_qtde_clientes(Escalonador *e) {
  int i; 
  int qntd = 0;

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

int e_conf_por_arquivo(Escalonador *e, char *nome_arq_conf) {

  FILE* file;
  Cliente *cliente;
	char buffer[100];
	int qntd_caixas, delta_t, line_count = 0;
	int* disc;

	file = fopen(nome_arq_conf, "r");
	if (file == NULL) {
		printf("[Error] Could not open file '%s'.\n", nome_arq_conf);
		return 0;
	}

	while(fgets(buffer, 100, file)) {
    line_count++;
    if (line_count == 1) {
			qntd_caixas = obter_qntd_caixas(buffer);
		} else if (line_count == 2) {
			delta_t = obter_tempo_por_oper(buffer);
		} else if (line_count == 3) {
			disc = obter_disc_escalonamento(buffer);
      e_inicializar(e, qntd_caixas, delta_t, disc[0], disc[1], disc[2], disc[3], disc[4]);
      free(disc);
		} else {
      cliente = obter_info_cliente(buffer);
			e_inserir_por_fila(e, obter_classe_num(cliente->classe), cliente->num_conta, cliente->qntd_opereracoes);
      free_client_ptr(cliente);
    }
	}

	fclose(file);
  
  return 1;
}

void e_rodar(Escalonador *e, char *nome_arq_in, char *nome_arq_out) {
  FILE* file;
  Log* tree;
  int i;

  log_inicializar(&tree);
  if (tree == NULL) {
    printf("[Error] Failed to initialize tree.\n");
    return;
  }

  if (e_conf_por_arquivo(e, nome_arq_in) == 0) {
    printf("[Error] Failed to configure.\n");
    return;
  };

  file = fopen(nome_arq_out, "w");
	if (file == NULL) {
		printf("[Error] Could not open file '%s'.\n", nome_arq_out);
		return;
	}

  escrever_chamadas(file, e, tree);
  escrever_media(file, tree);
  escrever_qntd_antedimentos(file, e);

  log_free(&tree);
  e_free_escalonador(e);
  fclose(file);
} 

void e_free_escalonador(Escalonador *e) {
  int i;
  for (i = 0; i < 5; i++) {
    free(e->filas[i]);
  }
  free(e->caixas);
}