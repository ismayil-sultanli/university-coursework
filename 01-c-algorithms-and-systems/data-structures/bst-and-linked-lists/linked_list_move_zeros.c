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
void moveZerosToEnd(Node** head) {
    Node* nonZeroHead = NULL;
    Node* nonZeroTail = NULL;
    Node* zeroHead = NULL;
    Node* zeroTail = NULL;
    Node* current = *head;
    Node* nextNode;

    while (current != NULL) {
        nextNode = current->next;  
        current->next = NULL;

        if (current->data != 0) {
            if (nonZeroHead == NULL) {
                nonZeroHead = current;
                nonZeroTail = current;
            } else {
                nonZeroTail->next = current;
                nonZeroTail = current;
            }
        } else {
            if (zeroHead == NULL) {
                zeroHead = current;
                zeroTail = current;
            } else {
                zeroTail->next = current;
                zeroTail = current;
            }
        }
        current = nextNode;
    }
    if (nonZeroHead == NULL) {
        *head = zeroHead;  
    } else {
        nonZeroTail->next = zeroHead;
        *head = nonZeroHead;
    }
}

void showMenu(void) {
    printf("\n========== MENU ==========\n");
    printf("1. Enter N and N numbers\n");
    printf("2. Print the list\n");
    printf("3. Move zeros to the end and print the list\n");
    printf("0. Exit\n");
    printf("==========================\n");
}

int main(void) {
    Node* head = NULL;
    int choice;

    printf("Program 3: moving zeros to the end of the list\n");

    do {
        showMenu();
        choice = readInt("Your choice: ");

        switch (choice) {
        case 1:
            createList(&head);
            printList(head);
            break;
        case 2:
            printList(head);
            break;
        case 3:
            if (head == NULL) {
                printf("The list is empty. Enter the numbers first.\n");
            } else {
                printf("Before: ");
                printList(head);
                moveZerosToEnd(&head);
                printf("After:  ");
                printList(head);
            }
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