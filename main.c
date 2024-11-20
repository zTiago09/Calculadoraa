#include <stdio.h>
#include <string.h>
#include "expressao.h"
#include "expressao.c"

int main() {
    Expressao expr;

    printf("Digite a expressao pos-fixada: ");
    fgets(expr.posFixa, sizeof(expr.posFixa), stdin);
    expr.posFixa[strcspn(expr.posFixa, "\n")] = '\0';

    converterPosfixaParaInfixaECalcular(&expr);

    printf("Expressao pos-fixada: %s\n", expr.posFixa);
    printf("Expressao infixa: %s\n", expr.inFixa);
    printf("Resultado: %.2f\n", expr.Valor);

    return 0;
}
