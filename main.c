#include <stdio.h>
#include <string.h>
#include "calculatorFunctions.h"

int main(void) {

    char input[256];

    printf("Welcome to the calculator, enter an expression!:");
    fgets(input, sizeof(input), stdin);

    Parser parser;
    parser.input = input;
    parser.pos = 0;

    parser.currentToken = getNextToken(parser.input, &parser.pos);

    double result = parseExpression(&parser);


    if (!parser.hasError) {
        printf("Result = %.2lf\n", result);
    }

    return 0;
}