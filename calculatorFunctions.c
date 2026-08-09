#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "calculatorFunctions.h"

//** ARITHMETIC OPERATIONS:
double add(double num1, double num2) {
    return num1 + num2;
}

double subtract(double num1, double num2) {
    return num1- num2;
}

double multiply(double num1, double num2) {
    return num1 * num2;
}

double divide(double num1, double num2) {
    if (num2 == 0) {
        printf("Error: Division by zero isn't possible!\n");
        return 0;
    }
    return num1 / num2;
}

//** TOKENIZING:
Token getNextToken(char *input, int *pos) {

    //** INITIALIZE TOKEN:
    Token t;
    t.type = TOKEN_INVALID;
    t.value = 0;
    t.text[0] = '\0';

    char c = input[*pos];

    //** WHITESPACE CHECK:
    while (isspace(c)) {
        (*pos)++;
        c = input[*pos];
    }

    if (c == '\0' || c == '\n') {
        t.type = TOKEN_END;
        return t;
    }

    //** NUMBER CHECK:
    if (isdigit(c) || c =='.') {
        char *endPtr;
        t.value = strtod(&input[*pos], &endPtr);
        t.type = TOKEN_NUMBER;
        int length = endPtr - &input[*pos];

        if (length > 31) {
            length = 31;
        }

        strncpy(t.text, &input[*pos], length);
        t.text[length] = '\0';

        *pos += (endPtr - &input[*pos]);

        return t;
    }

    //** IDENTIFIER CHECK:
    if (isalpha(c)) {
        int i = 0;
        while (isalnum(input[*pos]) && i < 31) {
            t.text[i] = input[*pos];
            i++;
            (*pos)++;
        }

        t.text[i] = '\0';
        t.type = TOKEN_IDENTIFIER;
        return t;
    }

    //** OPERATOR CHECKS:
    if (c == '+') {
        t.type = TOKEN_PLUS;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    if (c == '-') {
        t.type = TOKEN_MINUS;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    if (c == '*') {
        t.type = TOKEN_MULTIPLY;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    if (c == '/') {
        t.type = TOKEN_DIVIDE;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    if (c == '^') {
        t.type = TOKEN_POWER;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    if (c == '(') {
        t.type = TOKEN_LEFTPARENTHESIS;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    if (c == ')') {
        t.type = TOKEN_RIGHTPARENTHESIS;
        t.text[0] = c;
        t.text[1] = '\0';
        (*pos)++;
        return t;
    }

    //** FALLBACK:
    t.type = TOKEN_INVALID;
    t.text[0] = c;
    t.text[1] = '\0';
    (*pos)++;
    return t;
}

//** PARSING:
void initParser(Parser *parser, char *input) {
    parser->input = input;
    parser->pos = 0;
    parser->hasError = 0;
    parser->currentToken = getNextToken(parser->input, &parser->pos);
}

void advanceToken(Parser *parser) {
    parser->currentToken = getNextToken(parser->input, &parser->pos);
}

double parsePrimary(Parser *parser) {
    Token t = parser->currentToken;
    if (t.type == TOKEN_NUMBER) {
        double value = t.value;
        advanceToken(parser);
        return value;
    }

    if (t.type == TOKEN_IDENTIFIER) {
        char function[32];
        strcpy(function, t.text);
        advanceToken(parser);

        if (parser->currentToken.type != TOKEN_LEFTPARENTHESIS) {
            if (strcmp(function, "pi") == 0) {
                return M_PI;
            }
            else if (strcmp(function, "e") == 0) {
                return M_E;
            }
            else {
                parser->hasError = 1;
                printf("Error: unknown identifier '%s'\n", function);
                return 0;
            }
        }
        advanceToken(parser);

        double argument = parseExpression(parser);

        if (parser->currentToken.type != TOKEN_RIGHTPARENTHESIS) {
            parser->hasError = 1;
            printf("Error: expected ')'\n");
            return 0;
        }

        advanceToken(parser);

        if (strcmp (function, "sqrt") == 0) {
            return sqrt(argument);
        }
        else if (strcmp (function, "log") == 0) {
            if (argument <= 0) {
                parser->hasError = 1;
                printf("Error: log undefined for <=0 \n");
                return 0;
            }
            return log10(argument);
        }
        else if (strcmp (function, "ln") == 0) {
            if (argument <= 0) {
                parser->hasError = 1;
                printf("Error: ln undefined for <=0 \n");
                return 0;
            }
            return log(argument);
        }
        else if (strcmp (function, "sin") == 0) {
            return sin(argument * M_PI / 180);
        }
        else if (strcmp (function, "cos") == 0) {
            return cos(argument * M_PI / 180);
        }
        else if (strcmp (function, "tan") == 0) {
            return tan(argument * M_PI / 180);
        }
        else {
            parser->hasError = 1;
            printf("Error: unkown function '%s'\n", function);
            return 0;
        }

    }

    if (t.type == TOKEN_LEFTPARENTHESIS) {
        advanceToken(parser);
        double value = parseExpression(parser);

        if (parser->currentToken.type != TOKEN_RIGHTPARENTHESIS) {
            parser->hasError = 1;
            printf("Error: expected ')' \n");
            return 0;
        }

        advanceToken(parser);
        return value;
    }

    parser->hasError = 1;
    printf("Error: unexpected expression '%s'\n", t.text);
    return 0;
}

double parseExpression(Parser *parser) {
    double left = parseTerm(parser);

    while (parser->currentToken.type == TOKEN_PLUS || parser->currentToken.type == TOKEN_MINUS) {
        TokenType op = parser->currentToken.type;
        advanceToken(parser);

        double right = parseTerm(parser);

        if (op == TOKEN_PLUS) {
            left = add(left, right);
        } else if (op == TOKEN_MINUS) {
            left = subtract(left, right);
        }
    }
    return left;
}

double parseTerm(Parser *parser) {
    double left = parsePower(parser);

    while (parser->currentToken.type == TOKEN_MULTIPLY || parser->currentToken.type == TOKEN_DIVIDE) {
        TokenType op = parser->currentToken.type;
        advanceToken(parser);

        double right = parsePower(parser);

        if (op == TOKEN_MULTIPLY) {
            left = multiply(left, right);
        } else if (op == TOKEN_DIVIDE) {
            left = divide(left, right);
        }
    }
    return left;
}

double parseUnary (Parser *parser){

    if (parser->currentToken.type == TOKEN_PLUS) {
        advanceToken(parser);
        return parseUnary(parser);
    }

    if (parser->currentToken.type == TOKEN_MINUS) {
        advanceToken(parser);
        return -parseUnary(parser);
    }
    return parsePrimary(parser);
}

double parsePower (Parser *parser) {
    double left = parseUnary(parser);

    if (parser->currentToken.type == TOKEN_POWER) {
        advanceToken(parser);

        double right = parsePower(parser);

        return pow(left, right);
    }
    return left;
}