#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int v1;
    int v2;
    struct Node* next;
} Node;

void printList(Node* n);
void insert(Node** head_ref, int v1, int v2);
Node* intersection(Node* start1, Node* start2, int (*compare_f)(Node*, Node*));
int compare_by_v1(Node* Node1, Node* Node2);
int compare_by_v2(Node* Node1, Node* Node2);

void printList(Node* n) {
    while (n != NULL) {
        printf("v1=%d, v2=%d", n->v1, n->v2);
        n = n->next;
        if (n != NULL)
            printf(" → ");
    }
}

void insert(Node** head_ref, int v1, int v2) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->v1 = v1;
    new_node->v2 = v2;
    new_node->next = NULL;
    
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }
    
    Node* last = *head_ref;
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}

Node* intersection(Node* start1, Node* start2, int (*compare_f)(Node*, Node*)) {
    Node* result = NULL;
    Node* temp1 = start1;
    Node* temp2 = start2;

    while (temp1 != NULL && temp2 != NULL) {
        if (compare_f(temp1, temp2) == 0) {
            insert(&result, temp1->v1, temp1->v2);
            temp1 = temp1->next;
            temp2 = temp2->next;
        } else if (compare_f == compare_by_v1 && temp1->v1 < temp2->v1) {
            temp1 = temp1->next;
        } else if (compare_f == compare_by_v2 && temp1->v2 < temp2->v2) {
            temp1 = temp1->next;
        } else {
            temp2 = temp2->next;
        }
    }

    return result;
}

int compare_by_v1(Node* Node1, Node* Node2) {
    return (Node1->v1 == Node2->v1 && Node1->v2 == Node2->v2);
}

int compare_by_v2(Node* Node1, Node* Node2) {
    return (Node1->v2 == Node2->v2 && Node1->v1 == Node2->v1);
}

int main() {
    Node* list1 = NULL;
    Node* list2 = NULL;

    insert(&list1, 1, 10);
    insert(&list1, 2, 20);
    insert(&list1, 3, 5);

    insert(&list2, 2, 5);
    insert(&list2, 1, 25);
    insert(&list2, 4, 20);

    printf("Linked list 1: ");
    printList(list1);
    printf("\nLinked list 2: ");
    printList(list2);

    Node* result = intersection(list1, list2, compare_by_v1);
    printf("\n\nResult of comparison by v1: ");
    printList(result);

    result = intersection(list1, list2, compare_by_v2);
    printf("\nResult of comparison by v2: ");
    printList(result);

    return 0;
}