#include <stdio.h>
#include <stdlib.h>

int main() {
    double a, b;
    char opr;
    double res;

    printf("Enter expression: ");

    int input = scanf("%lf%c%lf", &a, &opr, &b);
    if (input != 3) {
        fprintf(stderr, "Error: Malformed input.\n");
        printf("Usage: [number][operator][number]\n");
        return EXIT_FAILURE;
    }

    switch (opr) {
        case '+':
            res = a + b;
            break;
        case '-':
            res = a - b;
            break;
        case '*':
            res = a * b;
            break;
        case '/':
            if (b == 0) {
                fprintf(stderr, "Error: undefined. Cannot perform division by zero.\n");
                return EXIT_FAILURE;
            }
            res = a / b;
            break;
        case '%':
            res = 0;
            break;
        default:
            fprintf(stderr, "Error: malformed input. Invalid operator.\n");
            printf("Usage: supported operators are +, -, *, /, %%.\n");
            return EXIT_FAILURE;
    }

    printf("%.3lf\n", res);

    return EXIT_SUCCESS;
}
