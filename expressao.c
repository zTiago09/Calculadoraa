
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

typedef struct Item {
    char expressao[512]; 
    double valor;        
    struct Item *Anterior;
} Item;

typedef struct {
    Item *Topo;
    int Tamanho;
} Pilha;

Pilha *criarPilha();
int estaVazia(Pilha *P);
Item *criarItem(const char *str, double valor);
void empilhar(Pilha *P, Item *I);
Item *desempilhar(Pilha *P);
void liberarPilha(Pilha *P);

Pilha *criarPilha() {
    Pilha *MinhaPilha = (Pilha *)malloc(sizeof(Pilha));
    if (MinhaPilha == NULL) {
        printf("ERRO: não foi possível alocar memória para a pilha!\n");
        exit(EXIT_FAILURE);
    }
    MinhaPilha->Topo = NULL;
    MinhaPilha->Tamanho = 0;
    return MinhaPilha;
}

int estaVazia(Pilha *P) {
    return P->Topo == NULL;
}

Item *criarItem(const char *str, double valor) {
    Item *NovoItem = (Item *)malloc(sizeof(Item));
    if (NovoItem == NULL) {
        printf("ERRO: não foi possível alocar memória para o item!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(NovoItem->expressao, str);
    NovoItem->valor = valor;
    NovoItem->Anterior = NULL;
    return NovoItem;
}

void empilhar(Pilha *P, Item *I) {
    I->Anterior = P->Topo;
    P->Topo = I;
    P->Tamanho++;
}

Item *desempilhar(Pilha *P) {
    if (estaVazia(P)) {
        printf("ERRO: a pilha está vazia.\n");
        return NULL;
    }
    Item *I = P->Topo;
    P->Topo = I->Anterior;
    P->Tamanho--;
    return I;
}

void liberarPilha(Pilha *P) {
    while (!estaVazia(P)) {
        Item *I = desempilhar(P);
        free(I);
    }
    free(P);
}

void converterPosfixaParaInfixaECalcular(Expressao *expressao) {
    Pilha *P = criarPilha();
    char temp[512] = "";
    const char *posFixa = expressao->posFixa;

    int i = 0;
    while (posFixa[i] != '\0') {
        
        if (isspace(posFixa[i])) {
            i++;
            continue;
        }

        
        if (isdigit(posFixa[i]) || (posFixa[i] == '-' && isdigit(posFixa[i + 1]))) {
            char operando[32];
            int j = 0;

            while (isdigit(posFixa[i]) || posFixa[i] == '.' || (j == 0 && posFixa[i] == '-')) {
                operando[j++] = posFixa[i++];
            }
            operando[j] = '\0';

            double valor = atof(operando);
            empilhar(P, criarItem(operando, valor));
        }
        
        else if (strchr("+-*/^", posFixa[i]) != NULL) {
            Item *op2 = desempilhar(P);
            Item *op1 = desempilhar(P);

            snprintf(temp, sizeof(temp), "(%s %c %s)", op1->expressao, posFixa[i], op2->expressao);

            double resultado = 0.0;
            switch (posFixa[i]) {
                case '+':
                    resultado = op1->valor + op2->valor;
                    break;
                case '-':
                    resultado = op1->valor - op2->valor;
                    break;
                case '*':
                    resultado = op1->valor * op2->valor;
                    break;
                case '/':
                    if (op2->valor == 0) {
                        printf("ERRO: divisão por zero!\n");
                        liberarPilha(P);
                        exit(EXIT_FAILURE);
                    }
                    resultado = op1->valor / op2->valor;
                    break;
                case '^':
                    resultado = pow(op1->valor, op2->valor);
                    break;
            }

            empilhar(P, criarItem(temp, resultado));
        }
        
        else if (posFixa[i] == 'r') { 
            i++; 
            Item *op = desempilhar(P);
            snprintf(temp, sizeof(temp), "sqrt(%s)", op->expressao);
            double resultado = sqrt(op->valor);
            empilhar(P, criarItem(temp, resultado));
        }
        else if (posFixa[i] == 's' && posFixa[i+1] == 'e') { 
            i += 2; 
            Item *op = desempilhar(P);
            snprintf(temp, sizeof(temp), "sen(%s)", op->expressao);
            double resultado = sin(op->valor * M_PI / 180.0); 
            empilhar(P, criarItem(temp, resultado));
        }
        else if (posFixa[i] == 'c' && posFixa[i+1] == 'o') { 
            i += 2; 
            Item *op = desempilhar(P);
            snprintf(temp, sizeof(temp), "cos(%s)", op->expressao);
            double resultado = cos(op->valor * M_PI / 180.0); 
            empilhar(P, criarItem(temp, resultado));
        }
        else if (posFixa[i] == 't') { 
            i++; 
            Item *op = desempilhar(P);
            snprintf(temp, sizeof(temp), "tan(%s)", op->expressao);
            double resultado = tan(op->valor * M_PI / 180.0); 
            empilhar(P, criarItem(temp, resultado));
        }
        else if (posFixa[i] == 'l') { 
            i++; 
            Item *op = desempilhar(P);
            snprintf(temp, sizeof(temp), "log10(%s)", op->expressao);
            double resultado = log10(op->valor);
            empilhar(P, criarItem(temp, resultado));
        }
        i++;
    }

    Item *finalItem = desempilhar(P);
    if (finalItem) {
        expressao->Valor = finalItem->valor;
        strcpy(expressao->inFixa, finalItem->expressao);
        free(finalItem);
    }

    liberarPilha(P);
}
