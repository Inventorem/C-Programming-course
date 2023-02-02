#include<stdio.h>
#include<stdlib.h>
#include "stack.h"

#define MAX_STACK_SIZE 1000

#define syntax "syntax error"
#define zero "division by zero"
#define calc "Calc error"

int Order(char symbol) {
    if (symbol == '*' || symbol == '/') return 1;
    if (symbol == '+' || symbol == '-') return 2;
    return 3;
}

void Calc(stack *stack_of_numbers, char operation) {
    int b = pop(stack_of_numbers);
    int a = pop(stack_of_numbers);
    switch (operation) {
        case '+':
            push(stack_of_numbers, a + b);
            break;
        case '-':
            push(stack_of_numbers, a - b);
            break;
        case '*':
            push(stack_of_numbers, a * b);
            break;
        case '/':
            if (b == 0) {
                printf(zero);
                exit(0);
            }
            push(stack_of_numbers, a / b);
            break;
        default:
            printf(calc);
            exit(0);
    }
}

int OperationCheck(char symbol) {
    return symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '(' || symbol == ')';
}

int DigitCheck(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

int GeneralCheck(char symbol) {
    return OperationCheck(symbol) || DigitCheck(symbol);
}

int Result(char *Input, int length) {
    stack Nums, Operations;
    Nums.top = -1, Operations.top = -1;
    for (int i = 0; i < length; i++) {
        switch (Input[i]) {
            case '(':
                push(&Operations, (int) Input[i]);
                break;
            case ')':
                if (i == 0 || Input[i - 1] == '(') {
                    printf(syntax);
                    exit(0);
                }
                char PreviousOperation;
                PreviousOperation = (char) pop(&Operations);
                while ((PreviousOperation != '(')) {
                    Calc(&Nums, PreviousOperation);
                    PreviousOperation = (char) pop(&Operations);
                }
                break;
            default:
                if (OperationCheck(Input[i])) {
                    while (Operations.top >= 0 && Order((char) peek(&Operations)) <= Order(Input[i]))
                        Calc(&Nums, (char) pop(&Operations));
                    push(&Operations, (int) Input[i]);
                } else {
                    int num = Input[i] - '0';
                    while (i < length && DigitCheck(Input[++i]))
                        num = num * 10 + (Input[i] - '0');
                    i--;
                    push(&Nums, num);
                }
                break;

        }
    }
    while (Operations.top >= 0)
        Calc(&Nums, (char) pop(&Operations));
    return pop(&Nums);
}

int main() {
    char Input[MAX_STACK_SIZE + 10];
    int length = 0;
    for (int i = 0; i < MAX_STACK_SIZE; ++i) {
        char symbol;
        if (!scanf("%c", &symbol))
            return 0;
        if (symbol == '\n')
            break;
        if (!GeneralCheck(symbol)) {
            printf(syntax);
            return 0;
        }
        Input[length++] = symbol;
    }
    printf("%d", Result(Input, length));
}
