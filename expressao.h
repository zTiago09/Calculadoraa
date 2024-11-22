
#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];
    char inFixa[512];
    double Valor;
} Expressao;

void converterPosfixaParaInfixaECalcular(Expressao *expr);

#endif
