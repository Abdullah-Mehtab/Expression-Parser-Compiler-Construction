// Compiler Construction || COMP-451 B
// Project (23rd May 2024)
// Group members: Abdullah Mehtab & Umm-e-Abiha
// Roll No#: 241607845 & 231522019
// Project Task 1

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

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
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
        char *tokenTypeStr;
        switch (tokens[i].type) {
            case IDENTIFIER:
                tokenTypeStr = "int literal found";
                break;
            case OPERATOR:
                tokenTypeStr = "Arithmetic operator";
                break;
            case SENTINEL:
                tokenTypeStr = "Sentinel";
                break;
            default:
                tokenTypeStr = "Unknown";
        }
        printf("%s: %s\n", tokenTypeStr, tokens[i].lexeme);
    }

    if (invalidTokenFound) {
        printf("Error! Invalid token '%c' encountered. Program terminated prematurely.\n", invalidToken);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("file name tou daal dou :v\n");
        return 1;
    }

    char input[64]; // Max token length = 64
    strcpy(input, argv[1]);

    printf("Program finds following tokens in the expression:\nExpression received: %s\n", input);

    lexicalAnalysis(input);

    return 0;
}

