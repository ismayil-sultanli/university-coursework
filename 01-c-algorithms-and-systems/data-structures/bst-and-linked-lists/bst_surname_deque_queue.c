#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char name[50];
    struct Node* left;
    struct Node* right;
};

struct ListNode {
    char name[50];
    struct ListNode* next;
};

struct ListNode* dequeHead = NULL;
struct ListNode* dequeTail = NULL;
struct ListNode* queueHead = NULL;
struct ListNode* queueTail = NULL;

void clearInput() {
    int c = getchar();
    while (c != '\n' && c != EOF)
        c = getchar();
}

void fixName(char name[]) {
    if (name[0] >= 'a' && name[0] <= 'z')
        name[0] = name[0] - 32;
    for (int i = 1; name[i] != '\0'; i++) {
        if (name[i] >= 'A' && name[i] <= 'Z')
            name[i] = name[i] + 32;
    }
}

void readName(char name[]) {
    scanf("%49s", name);
    clearInput();
    fixName(name);
}

struct Node* createNode(char name[]) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->name, name);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, char name[]) {
    if (root == NULL)
        return createNode(name);
    if (strcmp(name, root->name) < 0)
        root->left = insert(root->left, name);
    else if (strcmp(name, root->name) > 0)
        root->right = insert(root->right, name);
    return root;
}

struct Node* search(struct Node* root, char name[]) {
    if (root == NULL || strcmp(name, root->name) == 0)
        return root;
    if (strcmp(name, root->name) < 0)
        return search(root->left, name);
    return search(root->right, name);
}

struct Node* findMin(struct Node* root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

struct Node* deleteNode(struct Node* root, char name[]) {
    struct Node* temp;

    if (root == NULL)
        return NULL;

    if (strcmp(name, root->name) < 0)
        root->left = deleteNode(root->left, name);
    else if (strcmp(name, root->name) > 0)
        root->right = deleteNode(root->right, name);
    else {
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }
        temp = findMin(root->right);
        strcpy(root->name, temp->name);
        root->right = deleteNode(root->right, root->name);
    }
    return root;
}

void printTree(struct Node* root) {
    if (root != NULL) {
        printTree(root->left);
        printf("  %s (%d)\n", root->name, (int)strlen(root->name));
        printTree(root->right);
    }
}

void printTreeShape(struct Node* root, int level) {
    if (root != NULL) {
        printTreeShape(root->right, level + 1);
        for (int i = 0; i < level; i++)
            printf("        ");
        printf("%s\n", root->name);
        printTreeShape(root->left, level + 1);
    }
}

struct ListNode* createListNode(char name[]) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    strcpy(newNode->name, name);
    newNode->next = NULL;
    return newNode;
}

void addToFront(char name[]) {
    struct ListNode* newNode = createListNode(name);
    newNode->next = dequeHead;
    dequeHead = newNode;
    if (dequeTail == NULL)
        dequeTail = newNode;
}

void addToBack(char name[]) {
    struct ListNode* newNode = createListNode(name);
    if (dequeTail == NULL) {
        dequeHead = newNode;
        dequeTail = newNode;
    } else {
        dequeTail->next = newNode;
        dequeTail = newNode;
    }
}

void enqueue(char name[]) {
    struct ListNode* newNode = createListNode(name);
    if (queueTail == NULL) {
        queueHead = newNode;
        queueTail = newNode;
    } else {
        queueTail->next = newNode;
        queueTail = newNode;
    }
}

void printList(struct ListNode* head) {
    if (head == NULL) {
        printf("(empty)\n");
        return;
    }
    while (head != NULL) {
        printf("%s -> ", head->name);
        head = head->next;
    }
    printf("NULL\n");
}

void taskA(struct Node* root) {
    if (root != NULL) {
        taskA(root->left);
        if (strlen(root->name) > 6)
            addToFront(root->name);
        taskA(root->right);
    }
}

void taskB(struct Node* root) {
    if (root != NULL) {
        taskB(root->left);
        if (strlen(root->name) > 6 && strlen(root->name) < 10)
            addToBack(root->name);
        taskB(root->right);
    }
}

struct Node* findLong(struct Node* root) {
    struct Node* found;

    if (root == NULL)
        return NULL;
    found = findLong(root->left);
    if (found != NULL)
        return found;
    if (strlen(root->name) > 10)
        return root;
    return findLong(root->right);
}

struct Node* taskC(struct Node* root) {
    char name[50];
    struct Node* found = findLong(root);

    while (found != NULL) {
        strcpy(name, found->name);
        enqueue(name);
        root = deleteNode(root, name);
        found = findLong(root);
    }
    return root;
}

void taskD() {
    struct ListNode* temp;

    printf("Removing all items from the deque:\n");
    if (dequeHead == NULL)
        printf("  Deque is empty.\n");
    while (dequeHead != NULL) {
        temp = dequeHead;
        printf("  %s\n", temp->name);
        dequeHead = dequeHead->next;
        free(temp);
    }
    dequeTail = NULL;

    printf("Removing all items from the queue:\n");
    if (queueHead == NULL)
        printf("  Queue is empty.\n");
    while (queueHead != NULL) {
        temp = queueHead;
        printf("  %s\n", temp->name);
        queueHead = queueHead->next;
        free(temp);
    }
    queueTail = NULL;
}

struct Node* addExamples(struct Node* root) {
    char examples[12][50] = {"Anderson", "Baker", "Clark", "Davis",
                             "Evans", "Fisher", "Garcia", "Harris",
                             "Irwin", "Johnson", "King", "Lewis"};

    for (int i = 0; i < 12; i++)
        root = insert(root, examples[i]);
    printf("Example surnames were added.\n");
    return root;
}

void freeTree(struct Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void freeList(struct ListNode* head) {
    struct ListNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    struct Node* root = NULL;
    char name[50];
    int choice = -1;

    printf("Binary search tree and list demo\n");

    while (choice != 0) {
        printf("\n=== MENU ===\n");
        printf("1. Insert surname\n");
        printf("2. Search surname\n");
        printf("3. Delete surname\n");
        printf("4. Print tree\n");
        printf("5. Add example surnames\n");
        printf("6. a) Length > 6 -> beginning of deque\n");
        printf("7. b) 6 < length < 10 -> end of deque\n");
        printf("8. c) Length > 10 -> remove from tree, add to queue\n");
        printf("9. d) Remove and print all items of deque and queue\n");
        printf("0. End\n");
        printf("Choice (0-9): ");

        if (scanf("%d", &choice) != 1)
            choice = -1;
        if (feof(stdin))
            break;
        clearInput();

        switch (choice) {
        case 1:
            printf("Enter surname: ");
            readName(name);
            if (search(root, name) != NULL)
                printf("%s is already in the tree.\n", name);
            else {
                root = insert(root, name);
                printf("%s was added.\n", name);
            }
            break;
        case 2:
            printf("Enter surname to search: ");
            readName(name);
            if (search(root, name) != NULL)
                printf("%s was found.\n", name);
            else
                printf("%s was not found.\n", name);
            break;
        case 3:
            printf("Enter surname to delete: ");
            readName(name);
            if (search(root, name) != NULL) {
                root = deleteNode(root, name);
                printf("%s was deleted.\n", name);
            } else
                printf("%s was not found.\n", name);
            break;
        case 4:
            if (root == NULL)
                printf("Tree is empty.\n");
            else {
                printf("Surnames in alphabetical order (length):\n");
                printTree(root);
                printf("\nTree view (root is on the left):\n");
                printTreeShape(root, 0);
            }
            break;
        case 5:
            root = addExamples(root);
            break;
        case 6:
            taskA(root);
            printf("Deque: ");
            printList(dequeHead);
            break;
        case 7:
            taskB(root);
            printf("Deque: ");
            printList(dequeHead);
            break;
        case 8:
            root = taskC(root);
            printf("Queue: ");
            printList(queueHead);
            printf("Tree now:\n");
            if (root == NULL)
                printf("  Tree is empty.\n");
            printTree(root);
            break;
        case 9:
            taskD();
            break;
        case 0:
            printf("Thank you, bye!\n");
            break;
        default:
            printf("Bad choice\n");
        }
    }

    freeTree(root);
    freeList(dequeHead);
    freeList(queueHead);
    return 0;
}
