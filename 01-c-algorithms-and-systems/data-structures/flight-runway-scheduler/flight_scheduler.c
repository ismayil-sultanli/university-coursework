#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int flightCode;
    char departureCity[50];
    char arrivalCity[50];
    char runwayTime[6];
    int routeType;
    int runwayNumber;
} Flight;

typedef struct FlightNode {
    Flight flight;
    struct FlightNode *next;
} FlightNode;

void insertFlightSorted(FlightNode **head, Flight newFlight) {
    FlightNode *newNode = (FlightNode *)malloc(sizeof(FlightNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        return;
    }
    newNode->flight = newFlight;
    newNode->next = NULL;

    if (*head == NULL || (*head)->flight.flightCode >= newFlight.flightCode) {
        newNode->next = *head;
        *head = newNode;
    } else {
        FlightNode *current = *head;
        while (current->next != NULL && current->next->flight.flightCode < newFlight.flightCode) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

FlightNode* readAndSortFlights(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open file");
        return NULL;
    }

    FlightNode *head = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Flight newFlight;
        sscanf(line, "%d %s %s %s %d %d",
               &newFlight.flightCode,
               newFlight.departureCity,
               newFlight.arrivalCity,
               newFlight.runwayTime,
               &newFlight.routeType,
               &newFlight.runwayNumber);
        insertFlightSorted(&head, newFlight);
    }

    fclose(file);
    return head;
}

void printFlights(FlightNode *head) {
    FlightNode *current = head;
    while (current != NULL) {
        printf("%d %s %s %s %d %d\n",
               current->flight.flightCode,
               current->flight.departureCity,
               current->flight.arrivalCity,
               current->flight.runwayTime,
               current->flight.routeType,
               current->flight.runwayNumber);
        current = current->next;
    }
}

void writeRunwayFiles(FlightNode *head) {
    FlightNode *current = head;
    while (current != NULL) {
        char fileName[20];
        sprintf(fileName, "runway%d.txt", current->flight.runwayNumber);
        FILE *file = fopen(fileName, "a");
        if (file == NULL) {
            perror("Could not open file");
            current = current->next;
            continue;
        }

        fprintf(file, "%d %s %s %s %d %d\n",
                current->flight.flightCode,
                current->flight.departureCity,
                current->flight.arrivalCity,
                current->flight.runwayTime,
                current->flight.routeType,
                current->flight.runwayNumber);

        fclose(file);
        current = current->next;
    }
}

void printRunwayFiles() {
    for (int runwayNumber = 1; runwayNumber <= 8; runwayNumber++) {
        char fileName[20];
        sprintf(fileName, "runway%d.txt", runwayNumber);
        FILE *file = fopen(fileName, "r");
        if (file != NULL) {
            printf("File-%d: %s:\n", runwayNumber, fileName);
            char line[256];
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            printf("\n");
            fclose(file);
        }
    }
}

void createSampleFile() {
    FILE *file = fopen("flights.txt", "w");
    if (file == NULL) {
        perror("Could not create file");
        return;
    }
    fprintf(file, "3001 LONDON PARIS 13:00 0 1\n");
    fprintf(file, "2013 LONDON BERLIN 12:00 1 8\n");
    fprintf(file, "1015 BERLIN LONDON 20:00 1 2\n");
    fprintf(file, "1012 MADRID LONDON 14:00 0 1\n");
    fclose(file);
}

int main() {
    createSampleFile();

    const char *inputFile = "flights.txt";

    printf("Checking whether the file exists: %s\n", inputFile);

    FILE *file = fopen(inputFile, "r");
    if (file == NULL) {
        perror("Could not open file");
        return 1;
    }
    fclose(file);

    FlightNode *sortedFlights = readAndSortFlights(inputFile);

    if (sortedFlights != NULL) {
        printf("Sorted flights:\n");
        printFlights(sortedFlights);
        writeRunwayFiles(sortedFlights);
    }

    FlightNode *current = sortedFlights;
    while (current != NULL) {
        FlightNode *next = current->next;
        free(current);
        current = next;
    }

    printf("Created files and their contents:\n");
    printRunwayFiles();

    return 0;
}
