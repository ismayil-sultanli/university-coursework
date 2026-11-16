#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define INPUT_SIZE 10
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 100
#define BATCH_SIZE 32
#define MOMENTUM 0.9
#define BETA1 0.9
#define BETA2 0.999
#define EPSILON 1e-8
#define TRAIN_SIZE 80
#define TEST_SIZE 20

typedef struct {
    double data[INPUT_SIZE];
    double label;
} DataPoint;

double random_double() {
    return (rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

double mse_loss(double predicted, double actual) {
    return 0.5 * pow(predicted - actual, 2);
}

void gradient_descent_update(double *weights, double *gradients, int size) {
    for (int i = 0; i < size; i++) {
        weights[i] -= LEARNING_RATE * gradients[i];
    }
}

void sgd_update(double *weights, double *gradients, double *velocity, int size) {
    for (int i = 0; i < size; i++) {
        velocity[i] = MOMENTUM * velocity[i] + LEARNING_RATE * gradients[i];
        weights[i] -= velocity[i];
    }
}

void adam_update(double *weights, double *gradients, double *m, double *v, double *t, int size) {
    for (int i = 0; i < size; i++) {
        m[i] = BETA1 * m[i] + (1.0 - BETA1) * gradients[i];
        v[i] = BETA2 * v[i] + (1.0 - BETA2) * gradients[i] * gradients[i];
        double m_hat = m[i] / (1.0 - pow(BETA1, *t));
        double v_hat = v[i] / (1.0 - pow(BETA2, *t));
        weights[i] -= LEARNING_RATE * m_hat / (sqrt(v_hat) + EPSILON);
    }
    (*t)++;
}

double forward_pass(double *weights, DataPoint *data) {
    double sum = 0.0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        sum += weights[i] * data->data[i];
    }
    return sigmoid(sum);
}

void train_optimizer(int optimizer_type, const char *filename, int epoch_count, DataPoint *train_data, DataPoint *test_data) {
    double weights[INPUT_SIZE] = {0};
    double gradients[INPUT_SIZE] = {0};
    double velocity[INPUT_SIZE] = {0};
    double m[INPUT_SIZE] = {0};
    double v[INPUT_SIZE] = {0};
    double t = 1;
    FILE *file = fopen(filename, "w");
    double train_loss, test_loss;

    for (int epoch = 0; epoch < epoch_count; epoch++) {
        train_loss = 0.0;
        for (int i = 0; i < TRAIN_SIZE; i++) {
            train_loss += mse_loss(forward_pass(weights, &train_data[i]), train_data[i].label);
            for (int j = 0; j < INPUT_SIZE; j++) {
                gradients[j] = random_double();
            }
            switch (optimizer_type) {
                case 0:
                    gradient_descent_update(weights, gradients, INPUT_SIZE);
                    break;
                case 1:
                    sgd_update(weights, gradients, velocity, INPUT_SIZE);
                    break;
                case 2:
                    adam_update(weights, gradients, m, v, &t, INPUT_SIZE);
                    break;
            }
        }
        test_loss = 0.0;
        for (int i = 0; i < TEST_SIZE; i++) {
            test_loss += mse_loss(forward_pass(weights, &test_data[i]), test_data[i].label);
        }
        fprintf(file, "Epoch %d: Train Loss = %.6f, Test Loss = %.6f\n", epoch + 1, train_loss / TRAIN_SIZE, test_loss / TEST_SIZE);
        for (int i = 0; i < INPUT_SIZE; i++) {
            fprintf(file, "%.6f ", weights[i]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    srand(time(NULL));
    DataPoint train_data[TRAIN_SIZE], test_data[TEST_SIZE];
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            train_data[i].data[j] = random_double();
        }
        train_data[i].label = (i % 2 == 0) ? 1.0 : -1.0;
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            test_data[i].data[j] = random_double();
        }
        test_data[i].label = (i % 2 == 0) ? 1.0 : -1.0;
    }
    const char *gd_files[5] = {
        "gd_weights_1.txt", "gd_weights_2.txt", "gd_weights_3.txt", "gd_weights_4.txt", "gd_weights_5.txt"
    };
    const char *sgd_files[5] = {
        "sgd_weights_1.txt", "sgd_weights_2.txt", "sgd_weights_3.txt", "sgd_weights_4.txt", "sgd_weights_5.txt"
    };
    const char *adam_files[5] = {
        "adam_weights_1.txt", "adam_weights_2.txt", "adam_weights_3.txt", "adam_weights_4.txt", "adam_weights_5.txt"
    };
    for (int i = 0; i < 5; i++) {
        train_optimizer(0, gd_files[i], EPOCHS, train_data, test_data);
        train_optimizer(1, sgd_files[i], EPOCHS, train_data, test_data);
        train_optimizer(2, adam_files[i], EPOCHS, train_data, test_data);
    }
    printf("Training complete. 5 weight files for each optimizer generated.\n");
    return 0;
}