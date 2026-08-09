# Scientific Calculator

A command-line scientific calculator written in C during my first year of university.

The program parses and evaluates mathematical expressions while respecting operator precedence.

## Features

- Addition, subtraction, multiplication and division
- Parentheses and operator precedence
- Exponentiation
- Positive and negative values
- Trigonometric functions
- Square root
- Logarithmic functions
- Mathematical constants including pi and e
- Error handling for invalid expressions

## Implementation

The calculator uses tokenization and a recursive-descent parser to interpret mathematical expressions.

The project is split into:

- `main.c` - Handles program execution and user input
- `calculatorFunctions.c` - Contains the calculator, tokenization and parsing logic
- `calculatorFunctions.h` - Contains function declarations and data structures

## Built With

- C
- Standard C libraries
- `math.h`
