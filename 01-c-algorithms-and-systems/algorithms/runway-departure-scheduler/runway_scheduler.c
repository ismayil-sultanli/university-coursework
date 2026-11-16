#include <stdio.h>

int main() {
    int N, k;

    printf("Enter the number of planes (N): ");
    scanf("%d", &N);
    printf("Enter the minimum gap (k minutes): ");
    scanf("%d", &k);

    int planeNo[N], departureTimes[N];
    printf("Enter plane numbers and departure times (in minutes) in order:\n");
    for (int i = 0; i < N; i++) {
        printf("Plane %d number and departure time (in minutes): ", i + 1);
        scanf("%d %d", &planeNo[i], &departureTimes[i]);
    }

    int runway1PlaneNo[N], runway1Time[N], runway1Index = 0;
    int runway2PlaneNo[N], runway2Time[N], runway2Index = 0;

    int runway1LastTime = 0, runway2LastTime = 0;
    int runway1TotalDelay = 0, runway2TotalDelay = 0;

    for (int i = 0; i < N; i++) {
        if (i % 2 == 0) {
            if (departureTimes[i] >= runway1LastTime + k) {
                runway1LastTime = departureTimes[i];
            } else {
                runway1TotalDelay += (runway1LastTime + k - departureTimes[i]);
                runway1LastTime += k;
            }
            runway1PlaneNo[runway1Index] = planeNo[i];
            runway1Time[runway1Index] = runway1LastTime;
            runway1Index++;
        } else {
            if (departureTimes[i] >= runway2LastTime + k) {
                runway2LastTime = departureTimes[i];
            } else {
                runway2TotalDelay += (runway2LastTime + k - departureTimes[i]);
                runway2LastTime += k;
            }
            runway2PlaneNo[runway2Index] = planeNo[i];
            runway2Time[runway2Index] = runway2LastTime;
            runway2Index++;
        }
    }

    double runway1AverageDelay = runway1Index > 0 ? (double)runway1TotalDelay / runway1Index : 0.0;
    double runway2AverageDelay = runway2Index > 0 ? (double)runway2TotalDelay / runway2Index : 0.0;

    printf("\nRunway 1 assignments:\n");
    for (int i = 0; i < runway1Index; i++) {
        printf("Plane %d at minute %d\n", runway1PlaneNo[i], runway1Time[i]);
    }
    printf("Runway 1 average delay: %.2f minutes\n", runway1AverageDelay);

    printf("\nRunway 2 assignments:\n");
    for (int i = 0; i < runway2Index; i++) {
        printf("Plane %d at minute %d\n", runway2PlaneNo[i], runway2Time[i]);
    }
    printf("Runway 2 average delay: %.2f minutes\n", runway2AverageDelay);

    return 0;
}
