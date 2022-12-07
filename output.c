#include "output.h"

void chamar_cliente(FILE* file, Escalonador *e, Log *t, int num_caixa, int tempo) {
  int num_conta, num_operacoes;
  char *classe;

  e->caixas[num_caixa].timer += e_consultar_tempo_prox_cliente(e);
  e->caixas[num_caixa].atendimentos++;

  num_operacoes = e_consultar_prox_qtde_oper(e);
  num_conta = e_obter_prox_num_conta(e);

  log_registrar(&t, num_conta, e->fila_atual + 1, tempo, num_caixa, num_operacoes);
  escrever_atendimento(file, e, tempo, num_caixa, num_conta, num_operacoes);
}

void escrever_chamadas(FILE* file, Escalonador *e, Log *tree) {
  int qntd_clientes, i;
  int tempo = 0;

  qntd_clientes = e_consultar_qtde_clientes(e);

  while (qntd_clientes != 0) {
    for (i = 0; i < e->qntd_caixas; i++) {
      if (qntd_clientes != 0 && e->caixas[i].timer == tempo) {
        chamar_cliente(file, e, tree, i, tempo);
        qntd_clientes--;
      }
    }
    tempo++;
  }

  escrever_tempo_total(file, e, tempo);
}

void escrever_atendimento(FILE* file, Escalonador *e, int tempo, int num_caixa, int num_conta, int num_ops) {
  char* classe = obter_classe(e->fila_atual + 1);
  
  fprintf(
    file, 
    "T = %i min: Caixa %i chama da categoria %s cliente da conta %i para realizar %i operacao(oes).\n", 
    tempo, (num_caixa + 1), classe, num_conta, num_ops
  );
  
  free(classe);
};

void escrever_tempo_total(FILE* file, Escalonador *e, int tempo) {
  int i, tempo_total = 0;

  for (i = 0; i < e->qntd_caixas; i++) {
    tempo_total = (e->caixas[i].timer > tempo_total) ? e->caixas[i].timer : tempo_total;
  }

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

void escrever_qntd_antedimentos(FILE* file, Escalonador *e) {
  int i;
  for (i = 0; i < e->qntd_caixas; i++) {
    fprintf(file, "O caixa de número %i atendeu %i clientes.\n", i + 1, e->caixas[i].atendimentos);  
  }
}