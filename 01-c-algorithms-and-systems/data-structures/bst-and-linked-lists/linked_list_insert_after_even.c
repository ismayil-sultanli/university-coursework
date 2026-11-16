#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;
int readInt(const char* message) {
    int value;
    int c;
    int result;

    printf("%s", message);
    result = scanf("%d", &value);
    while (result != 1) {
        if (result == EOF) {
        
            printf("\nInput ended. Program stops.\n");
            exit(0);
        }
        printf("Wrong input! Please enter an integer: ");

        c = getchar();
        while (c != '\n' && c != EOF) {
            c = getchar();
        }
        result = scanf("%d", &value);
    }
    return value;
}
void addToEnd(Node** head, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    Node* current;

    if (newNode == NULL) {
        printf("Memory error!\n");
        return;
    }
    newNode->data = value;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}
void deleteList(Node** head) {
    while (*head != NULL) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
void createList(Node** head) {
    int n, i, value;

    deleteList(head);

    n = readInt("Enter the number of elements N: ");
    while (n <= 0) {
        n = readInt("N must be greater than 0. Enter N again: ");
    }

    for (i = 0; i < n; i++) {
        printf("a%d = ", i + 1);
        value = readInt("");
        addToEnd(head, value);
    }
    printf("The list is created.\n");
}
void printList(Node* head) {
    Node* current = head;

    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    printf("List: ");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}
int insertAfterLastEven(Node* head, int x) {
    Node* lastEven = NULL;
    Node* current = head;
    Node* newNode;
    while (current != NULL) {
        if (current->data % 2 == 0) {
            lastEven = current;
        }
        current = current->next;
    }

    if (lastEven == NULL) {
        return 0;
    }

    newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory error!\n");
        return 0;
    }
    newNode->data = x;
    newNode->next = lastEven->next;
    lastEven->next = newNode;
    return 1;
}
void showMenu(void) {
    printf("\n========== MENU ==========\n");
    printf("1. Enter a new list\n");
    printf("2. Add an element to the end\n");
    printf("3. Print the list\n");
    printf("4. Insert x after the last even element\n");
    printf("5. Clear the list\n");
    printf("0. Exit\n");
    printf("==========================\n");
}

int main(void) {
    Node* head = NULL;
    int choice, value, x;
 printf("Program 2: inserting x after the last even element\n");

    do {
        showMenu();
        choice = readInt("Your choice: ");

        switch (choice) {
        case 1:
            createList(&head);
            printList(head);
            break;
        case 2:
            value = readInt("Enter the number: ");
            addToEnd(&head, value);
            printList(head);
            break;
        case 3:
            printList(head);
            break;
        case 4:
            if (head == NULL) {
                printf("The list is empty. Enter a list first.\n");
            } else {
                x = readInt("Enter the number x: ");
                printf("Before: ");
                printList(head);
                if (insertAfterLastEven(head, x)) {
                    printf("Number %d was inserted.\n", x);
                    printf("After:  ");
                    printList(head);
                } else {
                    printf("There are no even elements in the list. x was not inserted.\n");
                }
            }
            break;
        case 5:
            deleteList(&head);
            printf("The list is cleared.\n");
            break;
        case 0:
            printf("Goodbye!\n");
            break;
        default:
            printf("There is no such option. Try again.\n");
        }
    } while (choice != 0);
    deleteList(&head);
    return 0;
}