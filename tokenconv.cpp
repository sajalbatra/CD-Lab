#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool isValidDelimiter(char ch) {
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}')
        return true;
    return false;
}

bool isValidOperator(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
        ch == '>' || ch == '<' || ch == '=')
        return true;
    return false;
}

bool isvalidIdentifier(char *str) {
    if (str[0] >= '0' && str[0] <= '9' || isValidDelimiter(str[0]))
        return false;
    return true;
}

bool isValidKeyword(char *str) {
    const char *keywords[] = { "if", "else", "while", "do", "break", "continue", "int", "double", 
                               "float", "return", "char", "case", "sizeof", "long", "short", 
                               "typedef", "switch", "unsigned", "void", "static", "struct", "goto" };
    int i;
    for (i = 0; i < 22; i++) {
        if (!strcmp(str, keywords[i]))
            return true;
    }
    return false;
}

bool isValidInteger(char *str) {
    int i, len = strlen(str);
    if (len == 0)
        return false;
    for (i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            if (str[i] == '-' && i == 0)
                continue;
            return false;
        }
    }
    return true;
}

bool isRealNumber(char *str) {
    int i, len = strlen(str);
    bool hasDecimal = false;
    if (len == 0)
        return false;
    for (i = 0; i < len; i++) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
            if (str[i] == '-' && i == 0)
                continue;
            return false;
        }
        if (str[i] == '.')
            hasDecimal = true;
    }
    return hasDecimal;
}

char *subString(char *str, int left, int right) {
    int i;
    char *subStr = (char *)malloc(sizeof(char) * (right - left + 2));
    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return subStr;
}

void detectTokens(char *str) {
    int left = 0, right = 0;
    int length = strlen(str);

    while (right <= length && left <= right) {
        if (!isValidDelimiter(str[right]))
            right++;
        if (isValidDelimiter(str[right]) && left == right) {
            if (isValidOperator(str[right]))
                printf("Valid operator : '%c'\n", str[right]);
            right++;
            left = right;
        } else if ((isValidDelimiter(str[right]) && left != right) || (right == length && left != right)) {
            char* subStr = subString(str, left, right - 1);
            
            if (isValidKeyword(subStr))
                printf("Valid keyword : '%s'\n", subStr);
            else if (isValidInteger(subStr))
                printf("Valid Integer : '%s'\n", subStr);
            else if (isRealNumber(subStr))
                printf("Real Number : '%s'\n", subStr);
            else if (isvalidIdentifier(subStr) && !isValidDelimiter(str[right - 1]))
                printf("Valid Identifier : '%s'\n", subStr);
            else if (!isvalidIdentifier(subStr) && !isValidDelimiter(str[right - 1]))
                printf("Invalid Identifier : '%s'\n", subStr);
            
            left = right;
            free(subStr);
        }
    }
}

int main() {
    char str[100] = "x=a+b*10; ";
    printf("The Program is : '%s' \n", str);
    printf("All Tokens are : \n");
    detectTokens(str);
    return 0;
}
