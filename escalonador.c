#include "escalonador.h"

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
    e->caixas[i].tempo_de_espera = 0;
  }
}

int e_inserir_por_fila(Escalonador *e, int classe, int num_conta, int qtde_operacoes) {
  return f_inserir(&e->filas[classe - 1], num_conta, qtde_operacoes);
}

int e_consultar_prox_fila(Escalonador *e) {
  int fila_num = e->fila_atual;
  int counter = 0;

  while(e->iteracao == e->ordem_de_chamada[fila_num] || f_num_elementos(&e->filas[fila_num]) == 0) {
    counter++;
    // Edge case: All remaining elements are from a single class;
    if (counter == 5) {
      e->iteracao = 0;
    }
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

int e_obter_prox_num_conta(Escalonador *e) {
  int conta, fila_anterior;

  fila_anterior = e->fila_atual;
  
  e->fila_atual = e_consultar_prox_fila(e);
  if (fila_anterior != e->fila_atual) {
    e->iteracao = 0;
  }

  conta = f_obter_proxima_chave(&e->filas[e->fila_atual]);
  // Increment scheduler iteration
  if (conta != -1) {
    e->iteracao++;
  }
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

int chamar_cliente(FILE* file, Escalonador *e, Log *t, int num_caixa, int tempo) {
  int classe_num, num_conta, num_operacoes, tempo_de_espera;
  char *classe;

  tempo_de_espera = e->caixas[num_caixa].tempo_de_espera - e->tempo_por_operacao; 
  // The cashier is free.
  if (e->caixas[num_caixa].atendimentos == 0 || tempo_de_espera == 0) {
    classe_num = e_consultar_prox_fila(e) + 1;
    classe = obter_classe(classe_num);
    num_operacoes = e_consultar_prox_qtde_oper(e);
    num_conta = e_consultar_prox_num_conta(e);

    log_registrar(&t, num_conta, classe_num, tempo, num_caixa, num_operacoes);

    fprintf(
      file, 
      "T = %i min: Caixa %i chama da categoria %s cliente da conta %i para realizar %i operacao(oes).\n", 
      tempo, (num_caixa + 1), classe, num_conta, num_operacoes
    );

    e->caixas[num_caixa].tempo_de_espera = num_operacoes * e->tempo_por_operacao;
    e->caixas[num_caixa].atendimentos++;

    free(classe);
    return 1;
  } else {
    e->caixas[num_caixa].tempo_de_espera -= e->tempo_por_operacao;
    return 0;
  }
}

void escrever_chamadas(FILE* file, Escalonador *e, Log *tree, int *tempo) {
  int qntd_clientes, i;
  
  qntd_clientes = e_consultar_qtde_clientes(e);

  while (qntd_clientes != 0) {
    for (i = 0; i < e->qntd_caixas; i++) {
      if(qntd_clientes != 0 && chamar_cliente(file, e, tree, i, *tempo)) {
        e_obter_prox_num_conta(e);
        qntd_clientes--;
      };
    }
    if (qntd_clientes != 0) *tempo += e->tempo_por_operacao; 
  }
}

void escrever_tempo_total(FILE* file, Escalonador *e, int tempo) {
  int tempo_restante, tempo_total, i;
  // Get the greatest remaining time.
  tempo_restante = e->caixas[0].tempo_de_espera;
  for (i = 0; i < e->qntd_caixas - 1; i++) {
    if (tempo_restante < e->caixas[i + 1].tempo_de_espera) {
      tempo_restante = e->caixas[i + 1].tempo_de_espera;
    }
  }
  tempo_total = tempo + tempo_restante;

  fprintf(file, "Tempo total de atendimento: %i minutos.\n", tempo_total);
}

void escrever_media(FILE* file, Log *t) {
  int i;
  char* classe;

  for (i = 0; i < 5; i++) {
    classe = obter_classe(i + 1);
    fprintf(
      file, 
      "Tempo medio de espera dos %i clientes %s: %.2f\n",
      log_obter_contagem_por_classe(&t, i + 1),
      classe,
      log_media_por_classe(&t, i + 1)
    );
    free(classe);
  }

  for (i = 0; i < 5; i++) {
    classe = obter_classe(i + 1);
    fprintf(
      file, 
      "Quantidade media de operacoes por cliente %s = %.2f\n",
      classe,
      log_media_ops_por_classe(&t, i + 1)
    );
    free(classe);
  }
}

void escrever_antendimentos(FILE* file, Escalonador *e) {
  int i;
  for (i = 0; i < e->qntd_caixas; i++) {
    fprintf(file, "O caixa de número %i atendeu %i clientes.\n", i + 1, e->caixas[i].atendimentos);  
  }
}

void e_rodar(Escalonador *e, char *nome_arq_in, char *nome_arq_out) {
  FILE* file;
  Log* tree;
  int qntd_clientes, i, tempo_total, tempo = 0;

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

  escrever_chamadas(file, e, tree, &tempo);
  escrever_tempo_total(file, e, tempo);
  escrever_media(file, tree);
  escrever_antendimentos(file, e);

  fclose(file);
} 