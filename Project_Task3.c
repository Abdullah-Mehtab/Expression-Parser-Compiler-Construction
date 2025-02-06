// Compiler Construction || COMP-451 B
// Project (23rd May 2024)
// Group members: Abdullah Mehtab & Umm-e-Abiha
// Roll No#: 241607845 & 231522019
// Project Task 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    IDENTIFIER,
    OPERATOR,
    SENTINEL,
    INVALID
} TokenType;

typedef struct {
    char lexeme[64]; // Max token length = 64
    TokenType type;
} Token;

Token tokens[32]; // Total amount of allowed tokens = 32
int tokenCount = 0;
int currentTokenIndex = 0;

char stack[128][64]; // Stack for storing lexemes (as integers)
char stac_chr[64]; // To store items being pushed to Stack (for displaying stack implementation table)
int top = -1; // Flag to see if string is accepted or not (last item must be $)
char inputStr[64]; // To store the input string
int identifierValues[32]; // To store the values of identifiers

int registerCount = 0; // To keep track of register numbers
FILE *assemblyFile; // File pointer for assembly code file

void push(char *value) {
    if (top < 128 - 1) {
        strcpy(stack[++top], value);
    } else {
        fprintf(stderr, "Error: Stack overflow.\n");
        exit(1);
    }
}

char* pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        fprintf(stderr, "Error: Stack underflow.\n");
        exit(1);
    }
}

void reverseString(char *str) {
    int length = strlen(str);  // Get the length of the string
    int i;
    char temp;
    
    for (i = 0; i < length / 2; i++) {
        // Swap characters
        temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

void syntaxError(char *msg) {
    printf("Syntax Error: %s\n", msg);
    printf("\nInput string is not accepted.\n");
    exit(1);
}

void printStack() {
    printf("\t\t|");
    for (int i = 0; i <= top; ++i) {
        printf(" %s", stack[i]);
    }
    printf("\n");
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int isValidIdentifier(char *id) {
    if (!isalpha(id[0]) && id[0] != '_') {
        return 0;
    }

    int len = strlen(id);
    for (int i = 1; i < len; ++i) {
        if (!isalnum(id[i]) && id[i] != '_') {
            return 0;
        }
    }

    return 1;
}

void lexicalAnalysis(char *input) {
    int index = 0;
    int invalidTokenFound = 0; // Flag
    char invalidToken = '\0';

    for (int i = 0; input[i] != '\0'; ++i) {
        char currentChar = input[i];

        if (isalpha(currentChar) || currentChar == '_') {
            tokens[tokenCount].type = IDENTIFIER;
            while (isalnum(currentChar) || currentChar == '_') {
                tokens[tokenCount].lexeme[index++] = currentChar;
                currentChar = input[++i];
            }
            tokens[tokenCount].lexeme[index] = '\0';
            index = 0;
            tokenCount++;
            i--;
        } else if (isOperator(currentChar)) {
            tokens[tokenCount].type = OPERATOR;
            tokens[tokenCount].lexeme[index++] = currentChar;
            tokens[tokenCount].lexeme[index] = '\0';
            index = 0;
            tokenCount++;
        } else if (currentChar == '$') {
            tokens[tokenCount].type = SENTINEL;
            tokens[tokenCount].lexeme[index++] = currentChar;
            tokens[tokenCount].lexeme[index] = '\0';
            index = 0;
            tokenCount++;
        } else {
            invalidTokenFound = 1;
            invalidToken = currentChar;
            break;
        }
    }

    for (int i = 0; i < tokenCount; ++i) {
        switch (tokens[i].type) {
            case IDENTIFIER:
                printf("int literal found: %s\n", tokens[i].lexeme);
                break;
            case OPERATOR:
                printf("Arithmetic operator: %s\n", tokens[i].lexeme);
                break;
            case SENTINEL:
                printf("Sentinel: %s\n", tokens[i].lexeme);
                break;
            default:
                break;
        }
    }

    if (invalidTokenFound) {
        printf("Error! Invalid token '%c' encountered. Program terminated prematurely.\n", invalidToken);
        exit(1);
    }
}

void match(TokenType expectedType) {
    if (tokens[currentTokenIndex].type == expectedType) {
        if (tokens[currentTokenIndex].type == OPERATOR) {
            strncat(stac_chr, &(inputStr + currentTokenIndex)[0], 1);
            reverseString(stac_chr);
            printf("%s$\t\t\t%s\t\t\t%s", stac_chr ,inputStr + currentTokenIndex + 1, "Push");
            printStack();
            currentTokenIndex++;
        } else {
            currentTokenIndex++;
        }
    } else {
        syntaxError("Token mismatch");
    }
}

void factor() {
    if (tokens[currentTokenIndex].type == IDENTIFIER) {
        // Retrieve value from identifierValues array
        int value = identifierValues[currentTokenIndex];
        char valueStr[64];
        sprintf(valueStr, "%d", value);
        push(valueStr);
        if ((inputStr + currentTokenIndex)[0] != '$') {
            strncat(stac_chr, &(inputStr + currentTokenIndex)[0], 1);
            reverseString(stac_chr);
        } 
        printf("%s$\t\t\t%s\t\t\t%s", stac_chr ,inputStr + currentTokenIndex + 1, "Push");
        printStack();

        fprintf(assemblyFile, "LI R%d, %s\n", registerCount, tokens[currentTokenIndex].lexeme);
        registerCount++;

        int size = strlen(stac_chr);
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
        }
        printf("%s$\t\t\t%s\t\t\t%s", stac_chr ,inputStr + currentTokenIndex + 1, "Pop");
        printStack();

        match(IDENTIFIER);
    } else if (tokens[currentTokenIndex].type == SENTINEL) {
        match(SENTINEL);
    } else {
        syntaxError("Invalid factor");
    }
}

void MUL_or_DIV() {
    factor();

    while (currentTokenIndex < tokenCount && 
          (tokens[currentTokenIndex].type == OPERATOR &&
          (tokens[currentTokenIndex].lexeme[0] == '*' ||
           tokens[currentTokenIndex].lexeme[0] == '/'))) {

        char op = tokens[currentTokenIndex].lexeme[0];
        match(OPERATOR);
        factor();
        char *bStr = pop();
        char *aStr = pop();
        int b = atoi(bStr);
        int a = atoi(aStr);

        int result;
        if (op == '*') {
            result = a * b;
            fprintf(assemblyFile, "MUL R%d, R%d, R%d\n", registerCount, registerCount-2, registerCount-1);
        } else {
            if (b == 0) {
                fprintf(stderr, "Error: Division by zero.\n");
                exit(1);
            }
            result = a / b;
            fprintf(assemblyFile, "DIV R%d, R%d, R%d\n", registerCount, registerCount-2, registerCount-1);
        }

        char resultStr[64];
        sprintf(resultStr, "%d", result);
        push(resultStr);

        registerCount++;

        int size = strlen(stac_chr);
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
        }
        printf("%s$\t\t\t%s\t\t\t%s", stac_chr, inputStr + currentTokenIndex, "Pop");
        printStack();
    }
}

void ADD_or_SUB() {
    MUL_or_DIV();

    while (currentTokenIndex < tokenCount && 
          (tokens[currentTokenIndex].type == OPERATOR &&
          (tokens[currentTokenIndex].lexeme[0] == '+' ||
          tokens[currentTokenIndex].lexeme[0] == '-'))) {

        char op = tokens[currentTokenIndex].lexeme[0];
        match(OPERATOR);
        MUL_or_DIV();
        char *bStr = pop();
        char *aStr = pop();
        int b = atoi(bStr);
        int a = atoi(aStr);

        int result;
        if (op == '+') {
            result = a + b;
            fprintf(assemblyFile, "ADD R%d, R%d, R%d\n", registerCount, registerCount-2, registerCount-1);
        } else {
            result = a - b;
            fprintf(assemblyFile, "SUB R%d, R%d, R%d\n", registerCount, registerCount-2, registerCount-1);
        }

        char resultStr[64];
        sprintf(resultStr, "%d", result);
        push(resultStr);

        registerCount++;

        int size = strlen(stac_chr);
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
        }
        printf("%s$\t\t\t%s\t\t\t%s", stac_chr, inputStr + currentTokenIndex, "Pop");
        printStack();
    }

    if (currentTokenIndex < tokenCount && tokens[currentTokenIndex].type == SENTINEL) {
        match(SENTINEL);
    } else {
        syntaxError("Invalid expression");
    }
}

void syntaxAnalysis(Token *tokens, int tokenCount) {
    printf("\nEnter integer values of the following identifiers:\n");
    for (int i = 0; i < tokenCount; ++i) {
        if (tokens[i].type == IDENTIFIER) {
            int value;
            printf("Value of %s: ", tokens[i].lexeme);
            if (scanf("%d", &value) != 1) {
                syntaxError("Invalid numerical value. Please enter a valid number.");
            }
            identifierValues[i] = value;
        }
    }

    assemblyFile = fopen("assembly_code.asm", "w");
    if (assemblyFile == NULL) {
        fprintf(stderr, "Error opening assembly code file.\n");
        exit(1);
    }
    fprintf(assemblyFile, "Assembly code generated for the expression: %s\n\n", inputStr);

    printf("\nThe stack implementation table for operator precedence parser for the given expression is as follows:\n");
    printf("Stack\t\t\tInput\t\t\tAction\n$\t\t\t%s\n",inputStr);

    ADD_or_SUB();

    if (top == 0) {
        printf("$\t\t\t$\t\t\tAccepted\n");
        printf("\nThe output of the given expression is: %s\n", stack[top]);
    } else {
        fprintf(stderr, "Error: Invalid expression.\n");
        printf("\nInput string is not accepted.\n");
        exit(1);
    }

    fclose(assemblyFile);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("file name tou daal dou :v\n");
        return 1;
    }

    strcpy(inputStr, argv[1]);

    printf("Program finds following tokens in the expression:\nExpression received: %s\n", inputStr);

    lexicalAnalysis(inputStr);

    syntaxAnalysis(tokens, tokenCount);

    printf("\n");
    // Display the contents of the assembly file on the console
    assemblyFile = fopen("assembly_code.asm", "r");
    char line[256];
    while (fgets(line, sizeof(line), assemblyFile)) {
        printf("%s", line);
    }
    fclose(assemblyFile);

    return 0;
}


