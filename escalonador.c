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
  return f_inserir(&e->filas[classe - 1], num_conta, qtde_operacoes);
}

int e_consultar_prox_fila(Escalonador *e) {
  int threshold_01 = e->ordem_de_chamada[0]; 
  int threshold_02 = e->ordem_de_chamada[1] + threshold_01;
  int threshold_03 = e->ordem_de_chamada[2] + threshold_02;
  int threshold_04 = e->ordem_de_chamada[3] + threshold_03; 
  int threshold_05 = e->ordem_de_chamada[4] + threshold_04;
  int ordem_de_chamada = e->iteracao % threshold_05; 
  int fila_num;

  // Queue 01;
  if (ordem_de_chamada < threshold_01) {
    fila_num = 0;
  // Queue 02;
  } else if (ordem_de_chamada < threshold_02) {
    fila_num = 1;
  // Queue 03;
  } else if (ordem_de_chamada < threshold_03) {
    fila_num = 2;
  // Queue 04;
  } else if (ordem_de_chamada < threshold_04) {
    fila_num = 3;
  // Queue 05;
  } else {
    fila_num = 4;
  }

  while(f_num_elementos(&e->filas[fila_num]) == 0) {
    fila_num = (fila_num + 1) % 5;
  }

  return fila_num;
}

int e_consultar_prox_num_conta(Escalonador *e) {
  int num_fila;

  num_fila = e_consultar_prox_fila(e);

  return f_consultar_proxima_chave(&e->filas[num_fila]);
}

int e_consultar_prox_qtde_oper(Escalonador *e) {
  int num_fila;

  num_fila = e_consultar_prox_fila(e);
  
  return f_consultar_proximo_valor(&e->filas[num_fila]);
}

int e_consultar_qtde_clientes(Escalonador *e) {
  int i; int qntd = 0;

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
  conta = f_obter_proxima_chave(&e->filas[num_fila]);

  // Increment scheduler iteration
  e->iteracao++;

  return conta;
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