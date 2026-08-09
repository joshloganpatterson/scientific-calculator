#ifndef CALCULATOR_PROJECT_CALCULATORFUNCTIONS_H
#define CALCULATOR_PROJECT_CALCULATORFUNCTIONS_H

//** ENUMS AND STRUCTS:
typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LEFTPARENTHESIS,
    TOKEN_RIGHTPARENTHESIS,
    TOKEN_IDENTIFIER,
    TOKEN_POWER,
    TOKEN_INVALID,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    double value;
    char text[32];
} Token;

typedef struct {
    char *input;
    int pos;
    Token currentToken;
    int hasError;
} Parser;

//** ARITHMETIC HEADERS:
double add(double num1, double num2);
double subtract(double num1, double num2);
double multiply(double num1, double num2);
double divide(double num1, double num2);

//** PARSING HEADERS:
void initParser(Parser *parser, char *input);
double parsePrimary(Parser *parser);
double parseExpression(Parser *parser);
double parseTerm(Parser *parser);
double parseUnary(Parser *parser);
double parsePower(Parser *parser);

//** TOKENIZING HEADERS:
Token getNextToken(char *input, int *pos);
void advanceToken(Parser *parser);

#endif