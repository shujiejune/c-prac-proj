#include <stdio.h>
#include <stdlib.h>

int is_integer(double n) {
    return n - (long long) n == 0;
}

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
            if (!is_integer(a) || !is_integer(b)) {
                fprintf(stderr, "Error: undefined. Remainder operator requires both operands to be integers.\n");
                return EXIT_FAILURE;
            }
            long long int_a = (long long) a;
            long long int_b = (long long) b;
            if (int_b == 0) {
                fprintf(stderr, "Error: undefined. Cannot perform remainder on zero.\n");
                return EXIT_FAILURE;
            }
            res = (double) (int_a % int_b);
            break;
        default:
            fprintf(stderr, "Error: malformed input. Invalid operator.\n");
            printf("Usage: supported operators are +, -, *, /, %%.\n");
            return EXIT_FAILURE;
    }

    printf("%.3lf\n", res);

    return EXIT_SUCCESS;
}
