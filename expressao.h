#ifndef EXPRESSAO_H

#define EXPRESSAO_H

typedef struct {

    char posFixa[512];     
    char inFixa[512];      
    float Valor;          

} Expressao;

char *getFormaInFixa(char *Str);    
float getValor(char *Str);          

#endif
