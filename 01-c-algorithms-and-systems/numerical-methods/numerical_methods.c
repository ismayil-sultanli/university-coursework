#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Matrix structure
typedef struct {
    double** elements;
    int size;
} Matrix;

// Function prototypes
double evaluate_function(char* func, double x);
double compute_derivative(char* func, double x);
bool has_real_root(char* func);

void read_matrix(Matrix* mat);
void print_matrix(Matrix* mat);
void swap_rows(Matrix* mat, int row1, int row2);

double** build_difference_table(double* x, double* y, int n);
double compute_interpolation(double** diffTable, double* x, double xp, int n);

void method_bisection();
void method_regula_falsi();
void method_newton_raphson();
void compute_inverse_matrix();
void method_gauss_elimination();
void method_gauss_seidel();
void compute_numerical_derivative();
void method_simpson();
void method_trapezoid();
void method_gregory_newton_interpolation();

void select_method(int option);

int main() {
    int option;

    do {
        printf("0 - Exit\n1 - Bisection method\n2 - Regula-Falsi method\n3 - Newton-Raphson method\n4 - Inverse of an NxN matrix\n5 - Gauss elimination\n6 - Gauss-Seidel method\n7 - Numerical derivative\n8 - Simpson's rule\n9 - Trapezoidal rule\n10 - Gregory-Newton interpolation\nChoice: ");
        scanf("%d", &option);

        select_method(option);
    } while (option != 0);

    return 0;
}

void select_method(int option) {
    switch(option) {
        case 0:
            exit(0);
        case 1:
            method_bisection();
            break;
        case 2:
            method_regula_falsi();
            break;
        case 3:
            method_newton_raphson();
            break;
        case 4:
            compute_inverse_matrix();
            break;
        case 5:
            method_gauss_elimination();
            break;
        case 6:
            method_gauss_seidel();
            break;
        case 7:
            compute_numerical_derivative();
            break;
        case 8:
            method_simpson();
            break;
        case 9:
            method_trapezoid();
            break;
        case 10:
            method_gregory_newton_interpolation();
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void method_bisection() {
    char func[256];
    double a, b, epsilon;
    int max_iterations, stop_criterion;

    printf("Function: ");
    scanf("%s", func);

    printf("Start: ");
    scanf("%lf", &a);
    printf("End: ");
    scanf("%lf", &b);
    printf("Epsilon: ");
    scanf("%lf", &epsilon);
    printf("Maximum iterations: ");
    scanf("%d", &max_iterations);
    printf("Stopping criterion:\n1. f(c) <= epsilon\n2. (end - start) / 2^n <= epsilon\nChoice: ");
    scanf("%d", &stop_criterion);

    double fa = evaluate_function(func, a);
    double fb = evaluate_function(func, b);

    if (fa * fb >= 0) {
        printf("The function must have opposite signs at a and b.\n");
        return;
    }

    for (int i = 1; i <= max_iterations; i++) {
        double c = (a + b) / 2;
        double fc = evaluate_function(func, c);

        printf("Iteration %d:\n", i);
        printf("start = %lf\n", a);
        printf("end = %lf\n", b);
        printf("mid = %lf\n", c);
        printf("f(start) = %lf\n", fa);
        printf("f(end) = %lf\n", fb);
        printf("f(mid) = %lf\n", fc);

        if (stop_criterion == 2 && (b - a) / pow(2, i) < epsilon) {
            printf("Result: %lf\n", c);
            return;
        }

        if (fabs(fc) < epsilon) {
            printf("Result: %lf\n", c);
            return;
        }

        if (fa * fc < 0) {
            b = c;
        } else {
            a = c;
        }
    }

    printf("Maximum number of iterations reached. Approximate root: %lf\n", (a + b) / 2);
}

void method_regula_falsi() {
    char func[256];
    double a, b, epsilon;
    int max_iterations;

    printf("Function: ");
    scanf("%s", func);

    if (!has_real_root(func)) {
        printf("No real root.\n");
        return;
    }

    printf("Start: ");
    scanf("%lf", &a);
    printf("End: ");
    scanf("%lf", &b);
    printf("Epsilon: ");
    scanf("%lf", &epsilon);
    printf("Maximum iterations: ");
    scanf("%d", &max_iterations);

    double fa = evaluate_function(func, a);
    double fb = evaluate_function(func, b);

    if (fa * fb >= 0) {
        printf("The function must have opposite signs at a and b.\n");
        return;
    }

    for (int i = 1; i <= max_iterations; i++) {
        double c = (a * fb - b * fa) / (fb - fa);
        double fc = evaluate_function(func, c);

        printf("Iteration %d:\n", i);
        printf("a = %lf\n", a);
        printf("b = %lf\n", b);
        printf("c = %lf\n", c);
        printf("f(a) = %lf\n", fa);
        printf("f(b) = %lf\n", fb);
        printf("f(c) = %lf\n\n", fc);

        if (fabs(fc) < epsilon) {
            printf("Result: %lf\n", c);
            return;
        }

        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }

    printf("Maximum number of iterations reached. Approximate root: %lf\n", (a * fb - b * fa) / (fb - fa));
}

void method_newton_raphson() {
    char func[256];
    double x0, epsilon;
    int max_iterations;

    printf("Function: ");
    scanf("%s", func);

    if (!has_real_root(func)) {
        printf("No real root.\n");
        return;
    }

    printf("Initial guess: ");
    scanf("%lf", &x0);
    printf("Epsilon: ");
    scanf("%lf", &epsilon);
    printf("Maximum iterations: ");
    scanf("%d", &max_iterations);

    double x = x0;
    for (int i = 1; i <= max_iterations; i++) {
        double fx = evaluate_function(func, x);
        double dfx = compute_derivative(func, x);

        if (fabs(dfx) < 1e-10) {
            printf("Derivative too small. No convergence.\n");
            return;
        }

        double x_new = x - fx / dfx;

        printf("Iteration %d:\n", i);
        printf("x = %lf\n", x);
        printf("f(x) = %lf\n", fx);
        printf("f'(x) = %lf\n", dfx);
        printf("x_new = %lf\n\n", x_new);

        if (fabs(x_new - x) < epsilon) {
            printf("Result: %lf\n", x_new);
            return;
        }

        x = x_new;
    }

    printf("Maximum number of iterations reached. Approximate root: %lf\n", x);
}

void compute_inverse_matrix() {
    int n;
    printf("Enter the matrix size: ");
    scanf("%d", &n);

    Matrix mat;
    mat.size = n;
    mat.elements = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        mat.elements[i] = (double*)malloc(n * sizeof(double));
    }

    printf("Enter the matrix elements row by row:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("[%d][%d]: ", i, j);
            scanf("%lf", &mat.elements[i][j]);
        }
    }

    double** augmented = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        augmented[i] = (double*)malloc(2 * n * sizeof(double));
        for (int j = 0; j < n; j++) {
            augmented[i][j] = mat.elements[i][j];
            augmented[i][j + n] = (i == j) ? 1 : 0;
        }
    }

    for (int i = 0; i < n; i++) {
        if (augmented[i][i] == 0) {
            for (int k = i + 1; k < n; k++) {
                if (augmented[k][i] != 0) {
                    for (int j = 0; j < 2 * n; j++) {
                        double temp = augmented[i][j];
                        augmented[i][j] = augmented[k][j];
                        augmented[k][j] = temp;
                    }
                    break;
                }
            }
        }

        double pivot = augmented[i][i];
        for (int j = 0; j < 2 * n; j++) {
            augmented[i][j] /= pivot;
        }

        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = augmented[k][i];
                for (int j = 0; j < 2 * n; j++) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }

    double** inverse = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        inverse[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            inverse[i][j] = augmented[i][j + n];
        }
    }

    printf("Inverse matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", inverse[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(mat.elements[i]);
        free(augmented[i]);
        free(inverse[i]);
    }
    free(mat.elements);
    free(augmented);
    free(inverse);
}

void method_gauss_elimination() {
    int n;
    printf("Enter the matrix size: ");
    scanf("%d", &n);

    Matrix mat;
    mat.size = n;
    mat.elements = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        mat.elements[i] = (double*)malloc((n + 1) * sizeof(double));
    }

    printf("Enter the augmented matrix row by row (including RHS):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            printf("[%d][%d]: ", i, j);
            scanf("%lf", &mat.elements[i][j]);
        }
    }

    for (int i = 0; i < n; i++) {
        if (mat.elements[i][i] == 0) {
            for (int k = i + 1; k < n; k++) {
                if (mat.elements[k][i] != 0) {
                    swap_rows(&mat, i, k);
                    break;
                }
            }
        }

        double pivot = mat.elements[i][i];
        for (int j = i; j <= n; j++) {
            mat.elements[i][j] /= pivot;
        }

        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = mat.elements[k][i];
                for (int j = i; j <= n; j++) {
                    mat.elements[k][j] -= factor * mat.elements[i][j];
                }
            }
        }
    }

    double* solution = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        solution[i] = mat.elements[i][n];
    }

    printf("Solution:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %lf\n", i, solution[i]);
    }

    for (int i = 0; i < n; i++) {
        free(mat.elements[i]);
    }
    free(mat.elements);
    free(solution);
}

void method_gauss_seidel() {
    int n, max_iterations;
    double tolerance;

    printf("Enter the matrix size: ");
    scanf("%d", &n);

    Matrix mat;
    mat.size = n;
    mat.elements = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        mat.elements[i] = (double*)malloc((n + 1) * sizeof(double));
    }

    printf("Enter the augmented matrix row by row (including RHS):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            printf("[%d][%d]: ", i, j);
            scanf("%lf", &mat.elements[i][j]);
        }
    }

    double* x = (double*)malloc(n * sizeof(double));
    printf("Enter the initial guess:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d]: ", i);
        scanf("%lf", &x[i]);
    }

    printf("Enter the tolerance: ");
    scanf("%lf", &tolerance);
    printf("Enter the maximum number of iterations: ");
    scanf("%d", &max_iterations);

    double* x_new = (double*)malloc(n * sizeof(double));
    for (int iter = 0; iter < max_iterations; iter++) {
        for (int i = 0; i < n; i++) {
            double sum = mat.elements[i][n];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sum -= mat.elements[i][j] * x[j];
                }
            }
            x_new[i] = sum / mat.elements[i][i];
        }

        bool converged = true;
        for (int i = 0; i < n; i++) {
            if (fabs(x_new[i] - x[i]) > tolerance) {
                converged = false;
                break;
            }
        }

        for (int i = 0; i < n; i++) {
            x[i] = x_new[i];
        }

        if (converged) {
            printf("Converged in %d iterations.\n", iter + 1);
            break;
        }
    }

    printf("Solution:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %lf\n", i, x[i]);
    }

    for (int i = 0; i < n; i++) {
        free(mat.elements[i]);
    }
    free(mat.elements);
    free(x);
    free(x_new);
}

void compute_numerical_derivative() {
    char func[256];
    double x, h;
    int method;

    printf("Function: ");
    scanf("%s", func);

    printf("Point at which to compute the derivative: ");
    scanf("%lf", &x);

    printf("Step size h: ");
    scanf("%lf", &h);

    printf("Choose a numerical derivative method:\n");
    printf("1. Forward difference\n");
    printf("2. Backward difference\n");
    printf("3. Central difference\n");
    printf("Enter your choice (1, 2 or 3): ");
    scanf("%d", &method);

    double derivative;

    switch (method) {
        case 1:
            derivative = (evaluate_function(func, x + h) - evaluate_function(func, x)) / h;
            break;
        case 2:
            derivative = (evaluate_function(func, x) - evaluate_function(func, x - h)) / h;
            break;
        case 3:
            derivative = (evaluate_function(func, x + h) - evaluate_function(func, x - h)) / (2 * h);
            break;
        default:
            printf("Invalid choice. Please choose 1, 2 or 3.\n");
            return;
    }

    printf("Approximate derivative of the function at x = %lf: %lf\n", x, derivative);
}

void method_simpson() {
    char func[256];
    double a, b;
    int n;
    int rule;

    printf("Function: ");
    scanf("%s", func);

    printf("Start: ");
    scanf("%lf", &a);
    printf("End: ");
    scanf("%lf", &b);
    printf("Number of intervals: ");
    scanf("%d", &n);

    printf("Choose the rule (1 for the 1/3 rule, 2 for the 3/8 rule): ");
    scanf("%d", &rule);

    if (rule == 1 && n % 2 != 0) {
        printf("The number of intervals must be even for the 1/3 rule.\n");
        return;
    }

    if (rule == 2 && n % 3 != 0) {
        printf("The number of intervals must be a multiple of 3 for the 3/8 rule.\n");
        return;
    }

    double h = (b - a) / n;
    double sum = evaluate_function(func, a) + evaluate_function(func, b);

    if (rule == 1) {
        for (int i = 1; i < n; i++) {
            double x = a + i * h;
            sum += (i % 2 == 0 ? 2 : 4) * evaluate_function(func, x);
        }
        printf("Result: %lf\n", (h / 3) * sum);
    } else {
        for (int i = 1; i < n; i++) {
            double x = a + i * h;
            sum += (i % 3 == 0 ? 2 : 3) * evaluate_function(func, x);
        }
        printf("Result: %lf\n", (3 * h / 8) * sum);
    }
}

void method_trapezoid() {
    char func[256];
    double a, b;
    int n;

    printf("Function: ");
    scanf("%s", func);
    printf("Interval start: ");
    scanf("%lf", &a);
    printf("Interval end: ");
    scanf("%lf", &b);
    printf("Number of trapezoids: ");
    scanf("%d", &n);

    double h = (b - a) / n;
    double sum = 0.5 * (evaluate_function(func, a) + evaluate_function(func, b));

    for (int i = 1; i < n; i++) {
        sum += evaluate_function(func, a + i * h);
    }

    printf("Result: %lf\n", h * sum);
}

void method_gregory_newton_interpolation() {
    int n;
    printf("Enter the number of data points: ");
    scanf("%d", &n);

    double* x = (double*)malloc(n * sizeof(double));
    double* y = (double*)malloc(n * sizeof(double));
    printf("Enter the data points as (x y), separated by spaces:\n");
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &x[i], &y[i]);
    }

    double xp;
    printf("Enter the interpolation point: ");
    scanf("%lf", &xp);

    double** diffTable = build_difference_table(x, y, n);
    double result = compute_interpolation(diffTable, x, xp, n);

    printf("Interpolated value at %lf: %lf\n", xp, result);

    for (int i = 0; i < n; i++) {
        free(diffTable[i]);
    }
    free(diffTable);
    free(x);
    free(y);
}

double** build_difference_table(double* x, double* y, int n) {
    double** diffTable = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        diffTable[i] = (double*)malloc(n * sizeof(double));
        diffTable[i][0] = y[i];
    }

    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            diffTable[i][j] = diffTable[i + 1][j - 1] - diffTable[i][j - 1];
        }
    }

    return diffTable;
}

double compute_interpolation(double** diffTable, double* x, double xp, int n) {
    double sum = diffTable[0][0];
    double term = 1;
    double h = x[1] - x[0];
    double s = (xp - x[0]) / h;

    printf("h = %lf, s = %lf\n", h, s);

    for (int i = 1; i < n; i++) {
        term *= (s - (i - 1)) / i;
        printf("term[%d] = %lf, diffTable[0][%d] = %lf\n", i, term, i, diffTable[0][i]);
        sum += term * diffTable[0][i];
        printf("sum after term[%d] = %lf\n", i, sum);
    }

    return sum;
}

double evaluate_function(char* func, double x) {
    char signs[20];
    int sign_count = 0;

    if (func[0] != '-') signs[sign_count++] = '+';

    for (int i = 0; func[i] != '\0'; i++) {
        if (func[i] == '+') signs[sign_count++] = '+';
        if (func[i] == '-') signs[sign_count++] = '-';
    }

    int term_count = sign_count;

    char terms[term_count][20];
    char buffer[strlen(func) + 1];
    strcpy(buffer, func);

    char* token = strtok(buffer, "+-");
    int i = 0;
    while (token != NULL && i < term_count) {
        strcpy(terms[i], token);
        token = strtok(NULL, "+-");
        i++;
    }

    double result = 0;
    for (int i = 0; i < term_count; i++) {
        double coefficient = 1.0;
        int exponent = 0;
        char func_type[10] = "";
        char* ptr = terms[i];

        if (isdigit(*ptr) || *ptr == '.') {
            coefficient = atof(ptr);
            while (isdigit(*ptr) || *ptr == '.') ptr++;
            if (*ptr == '*') ptr++;   /* allow both "2x" and "2*x" */
        }

        if (*ptr == 'x') {
            if (*(ptr + 1) == '^') exponent = atoi(ptr + 2);
            else exponent = 1;
        } else if (strncmp(ptr, "sin", 3) == 0) {
            strcpy(func_type, "sin");
        } else if (strncmp(ptr, "cos", 3) == 0) {
            strcpy(func_type, "cos");
        } else if (strncmp(ptr, "tan", 3) == 0) {
            strcpy(func_type, "tan");
        } else if (strncmp(ptr, "exp", 3) == 0) {
            strcpy(func_type, "exp");
        }

        double term_value = 0;
        if (strcmp(func_type, "sin") == 0) {
            term_value = coefficient * sin(x);
        } else if (strcmp(func_type, "cos") == 0) {
            term_value = coefficient * cos(x);
        } else if (strcmp(func_type, "tan") == 0) {
            term_value = coefficient * tan(x);
        } else if (strcmp(func_type, "exp") == 0) {
            term_value = coefficient * exp(x);
        } else {
            term_value = coefficient * pow(x, exponent);
        }

        if (signs[i] == '-') term_value *= -1;
        result += term_value;
    }

    return result;
}

double compute_derivative(char* func, double x) {
    double h = 1e-5;
    return (evaluate_function(func, x + h) - evaluate_function(func, x - h)) / (2 * h);
}

bool has_real_root(char* func) {
    double test_points[] = {-10, -5, 0, 5, 10};
    int point_count = sizeof(test_points) / sizeof(test_points[0]);
    double prev_value = evaluate_function(func, test_points[0]);

    for (int i = 1; i < point_count; i++) {
        double curr_value = evaluate_function(func, test_points[i]);
        if (prev_value * curr_value <= 0) {
            return true;
        }
        prev_value = curr_value;
    }

    return false;
}

void read_matrix(Matrix* mat) {
    printf("Enter the matrix size: ");
    scanf("%d", &mat->size);

    mat->elements = (double**)malloc(mat->size * sizeof(double*));
    for (int i = 0; i < mat->size; i++) {
        mat->elements[i] = (double*)malloc(mat->size * sizeof(double));
    }

    printf("Enter the matrix elements row by row:\n");
    for (int i = 0; i < mat->size; i++) {
        for (int j = 0; j < mat->size; j++) {
            printf("[%d][%d]: ", i, j);
            scanf("%lf", &mat->elements[i][j]);
        }
    }
}

void print_matrix(Matrix* mat) {
    for (int i = 0; i < mat->size; i++) {
        for (int j = 0; j < mat->size; j++) {
            printf("%lf ", mat->elements[i][j]);
        }
        printf("\n");
    }
}

void swap_rows(Matrix* mat, int row1, int row2) {
    double* temp = mat->elements[row1];
    mat->elements[row1] = mat->elements[row2];
    mat->elements[row2] = temp;
}