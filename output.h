#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include "escalonador.h"

void escrever_atendimento(FILE* file, Escalonador *e, int tempo, int num_caixa, int num_conta, int num_ops);
void escrever_tempo_total(FILE* file, Escalonador *e, int tempo);
void escrever_chamadas(FILE* file, Escalonador *e, Log *tree);
void escrever_media(FILE* file, Log *t);
void escrever_qntd_antedimentos(FILE* file, Escalonador *e);

#endif /* OUTPUT_H */