
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct QueueNode {
    int customerId;
    struct QueueNode *next;
} QueueNode;
typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;
typedef struct StackNode {
    int customerId;
    struct StackNode *next;
} StackNode;
typedef struct {
    StackNode *top;
    int size;
} Stack;

/*
Initialises a newly created queue.
The queue has no customers yet, so front and rear are set to NULL.
*/

void initQueue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

}

/*
Checks whether the normal waiting queue is empty.
Returns 1 if it has no elements, otherwise 0.
*/

int isQueueEmpty(Queue *q) {
    return q->size == 0;
}

/*
Adds a newly calling customer to the end of the waiting queue.
A new node is allocated for every customer added.
*/


void enqueue(Queue *q, int customerId) {
    QueueNode *node;
    node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) {
        fprintf(stderr, "Error: memory allocation failed (enqueue)\n");
        exit(1);
    }
    node->customerId = customerId;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = node;
        q->rear  = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;



}
/*

Removes the customer who has waited longest in the normal queue.
Returns -1 when the queue is empty.
*/

int dequeue(Queue *q) {
    int id;
    QueueNode *temp;
    if (q->front == NULL) {
        return -1;
    }
    id = q->front->customerId;
    temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return id;
}



/*
Frees all queue nodes at the end of the program,
so memory is released even if customers are still waiting.
*/


void freeQueue(Queue *q) {
    QueueNode *temp;
    while (q->front != NULL) {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
    q->size = 0;
}


/*
Initialises the transfer stack as empty;
there is no customer on top of the stack at the start.
*/


void initStack(Stack *s) {
    s->top  = NULL;
    s->size = 0;


}
/*
Checks whether any customer is waiting in the transfer stack.
Returns 1 if it has no elements, otherwise 0.
*/


int isStackEmpty(Stack *s) {
    return s->size == 0;
}


/*
Pushes a customer who requested a transfer onto the stack.
The most recently added customer is served first (LIFO).
*/


void push(Stack *s, int customerId) {
    StackNode *node;
    node = (StackNode *)malloc(sizeof(StackNode));
    if (node == NULL) {
        fprintf(stderr, "Error: memory allocation failed (push)\n");
        exit(1);
    }
    node->customerId = customerId;
    node->next = s->top;
    s->top = node;
    s->size++;
}


/*
Pops the most recently added customer from the transfer stack.
Returns -1 when the stack is empty.
*/


int pop(Stack *s) {
    int id;
    StackNode *temp;
    if (s->top == NULL) {
        return -1;
    }
    id = s->top->customerId;
    temp = s->top;
    s->top = s->top->next;
    free(temp);
    s->size--;
    return id;
}


/*
Frees every node of the transfer stack.
Used to release dynamic memory when the program ends.
*/


void freeStack(Stack *s) {
    StackNode *temp;
    while (s->top != NULL) {
        temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
    s->size = 0;


}

/*
Reads the next integer from the file.
On success it stores the number at the given address and returns 1.
*/


int readInt(FILE *fp, int *value) {
    int c;
    int next;
    int sign;
    int result;
    int hasDigit;
    int tokenFound;
    c = 0;
    tokenFound = 0;
    while (!tokenFound && (c = fgetc(fp)) != EOF) {
        if (isspace(c)) {
            /* Whitespace is skipped until a number is found. */
        } else if (c == 0xEF) {
            int c2;
            int c3;
            c2 = fgetc(fp);
            c3 = fgetc(fp);
            if (!(c2 == 0xBB && c3 == 0xBF)) {
                if (c3 != EOF) {
                    ungetc(c3, fp);
                }
                if (c2 != EOF) {
                    ungetc(c2, fp);
                }
                tokenFound = 1;
            }
        } else if (c == '/') {
            next = fgetc(fp);
            if (next == '/') {
                while ((c = fgetc(fp)) != EOF && c != '\n') {

                    /* The comment up to the end of the line is skipped. */
                }
            } else {
                if (next != EOF) {
                    ungetc(next, fp);
                }
                tokenFound = 1;
            }
        } else {
            tokenFound = 1;
        }
    }
    if (!tokenFound) {
        return 0;
    }
    sign = 1;
    if (c == '-' || c == '+') {
        if (c == '-') {
            sign = -1;
        }
        c = fgetc(fp);
    }
    if (c == EOF || !isdigit(c)) {
        return 0;
    }
    result = 0;
    hasDigit = 0;
    while (c != EOF && isdigit(c)) {
        result = result * 10 + (c - '0');
        hasDigit = 1;
        c = fgetc(fp);
    }
    if (c != EOF) {
        ungetc(c, fp);
    }
    *value = sign * result;
    return hasDigit;
}


/*
Finds the free agent with the smallest ID.
Returns -1 if no agent is free.
*/

int findFreeAgent(int *agents, int agentCount) {
    int i;
    for (i = 0; i < agentCount; i++) {
        if (agents[i] == -1) {
            return i;
        }
    }
    return -1;

}

/*
Finds which agent is talking to the given customer.
Returns -1 if the customer is not in an active call.
*/

int findAgentOfCustomer(int *agents, int agentCount, int customerId) {
    int i;
    for (i = 0; i < agentCount; i++) {
        if (agents[i] == customerId) {
            return i;
        }
    }
    return -1;
}

/*
Connects the next waiting customer to a freed agent.
Customers in the transfer stack are served before the normal queue.
*/

void connectNext(int agentId, int *agents, Stack *transferStack,
                 Queue *waitQueue, int eventTime) {
    int customerId;
    if (!isStackEmpty(transferStack)) {
        customerId = pop(transferStack);
        agents[agentId] = customerId;
        printf("%d %d %d\n", eventTime, customerId, agentId);
    } else if (!isQueueEmpty(waitQueue)) {
        customerId = dequeue(waitQueue);
        agents[agentId] = customerId;
        printf("%d %d %d\n", eventTime, customerId, agentId);
    }
}

/*
Runs the simulation by processing the events read from the file in order.
Agent states, the waiting queue and the transfer stack are used together here.
*/

void processEvents(FILE *fp, int agentCount, int eventCount) {
    int *agents;
    Queue waitQueue;
    Stack transferStack;
    int i;
    int eventTime;
    int eventType;
    int param;
    int agentId;
    int freeId;
    int inputOk;

    /* Agent states (free or busy) are kept in a dynamic array. */

    agents = (int *)malloc(agentCount * sizeof(int));
    if (agents == NULL) {
        fprintf(stderr, "Error: memory allocation failed (agents array)\n");
        return;
    }

    /* All agents start as free. */

    for (i = 0; i < agentCount; i++) {
        agents[i] = -1;
    }
    initQueue(&waitQueue);
    initStack(&transferStack);
    
    /* Events are processed one by one in file order. */

    inputOk = 1;
    for (i = 0; i < eventCount && inputOk; i++) {
        if (!readInt(fp, &eventTime) ||
            !readInt(fp, &eventType) ||
            !readInt(fp, &param)) {
            inputOk = 0;
        } else {
            if (eventType == 1) {
                
                /* New call: find a free agent, otherwise put the customer in the queue. */

                freeId = findFreeAgent(agents, agentCount);
                if (freeId != -1) {
                    agents[freeId] = param;
                    printf("%d %d %d\n", eventTime, param, freeId);
                } else {
                    enqueue(&waitQueue, param);
                }
            } else if (eventType == 2) {
                
                /* Call ended: the agent becomes free and the next waiting customer is connected. */

                agentId = param;
                agents[agentId] = -1;
                connectNext(agentId, agents, &transferStack, &waitQueue, eventTime);
            } else if (eventType == 3) {
                
                /* Transfer request: the customer goes to the stack and the agent is reused. */

                agentId = findAgentOfCustomer(agents, agentCount, param);
                if (agentId != -1) {
                    agents[agentId] = -1;
                    push(&transferStack, param);
                    connectNext(agentId, agents, &transferStack, &waitQueue, eventTime);
                }
            }
        }
    }
    
    /* Allocated memory is released when the simulation ends. */

    freeQueue(&waitQueue);
    freeStack(&transferStack);
    free(agents);

}

/*
Opens the input file and starts the simulation.
If no file name is given on the command line, the user is asked for a relative path.
*/

int main(int argc, char *argv[]) {
    FILE *fp;
    int agentCount;
    int eventCount;
    char *firstToken;
    fp = NULL;
    firstToken = NULL;
    
    /* If a file name was given on the command line, open it directly. */

    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "Error: could not open file '%s'.\n", argv[1]);
            return 1;
        }
        if (!readInt(fp, &agentCount) || !readInt(fp, &eventCount)) {
            fprintf(stderr, "Error: could not read the first two lines of the input file.\n");
            fclose(fp);
            return 1;
        }
    } else {
        
        /* The file name is read from input, so this buffer is allocated dynamically. */

        firstToken = (char *)malloc(256 * sizeof(char));
        if (firstToken == NULL) {
            fprintf(stderr, "Error: memory allocation failed (file name)\n");
            return 1;
        }
        fprintf(stderr, "Enter the input file name: ");
        fflush(stderr);
        if (scanf("%255s", firstToken) != 1) {
            fprintf(stderr, "Error: could not read input.\n");
            free(firstToken);
            return 1;
        }
        fp = fopen(firstToken, "r");
        if (fp == NULL) {
            fprintf(stderr, "Error: could not open file '%s'.\n", firstToken);
            free(firstToken);
            return 1;
        }
        if (!readInt(fp, &agentCount) || !readInt(fp, &eventCount)) {
            fprintf(stderr, "Error: could not read the first two lines of the input file.\n");
            fclose(fp);
            free(firstToken);
            return 1;
        }
        free(firstToken);
        firstToken = NULL;
    }
    
    /* After reading the agent and event counts, process the events. */

    processEvents(fp, agentCount, eventCount);
    fclose(fp);
    return 0;
}
