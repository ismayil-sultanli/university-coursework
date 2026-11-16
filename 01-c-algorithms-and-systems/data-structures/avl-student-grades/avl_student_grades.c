#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 50
#define LINE_LEN 256
#define DEFAULT_FILE "students.txt"
#define REPORT_FILE "report.txt"

typedef struct {
    int id;
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
    double grade;
} Student;

typedef struct Node {
    Student data;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int height(Node *n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

void updateHeight(Node *n)
{
    int hl = height(n->left);
    int hr = height(n->right);
    n->height = 1 + (hl > hr ? hl : hr);
}

int getBalance(Node *n)
{
    return height(n->left) - height(n->right);
}

Node *createNode(Student s)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        printf("Error: not enough memory!\n");
        exit(1);
    }
    n->data = s;
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    return n;
}

Node *rotateRight(Node *y)
{
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    updateHeight(y);
    updateHeight(x);
    return x;
}

Node *rotateLeft(Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    updateHeight(x);
    updateHeight(y);
    return y;
}

Node *rebalance(Node *node)
{
    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1) {
        if (getBalance(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1) {
        if (getBalance(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

Node *insert(Node *root, Student s)
{
    if (root == NULL)
        return createNode(s);

    if (s.id < root->data.id)
        root->left = insert(root->left, s);
    else if (s.id > root->data.id)
        root->right = insert(root->right, s);
    else
        return root;

    return rebalance(root);
}

Node *searchById(Node *root, int id)
{
    while (root != NULL && root->data.id != id) {
        if (id < root->data.id)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

Node *deleteNode(Node *root, int id)
{
    if (root == NULL)
        return NULL;

    if (id < root->data.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->data.id)
        root->right = deleteNode(root->right, id);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node *child = (root->left != NULL) ? root->left : root->right;
            free(root);
            return child;
        }
        Node *succ = root->right;
        while (succ->left != NULL)
            succ = succ->left;
        root->data = succ->data;
        root->right = deleteNode(root->right, succ->data.id);
    }
    return rebalance(root);
}

void freeTree(Node *root)
{
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int countNodes(Node *root)
{
    if (root == NULL)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void printLine(FILE *out)
{
    fprintf(out, "+--------+----------------------+----------------------+-------+\n");
}

void printHeader(FILE *out)
{
    printLine(out);
    fprintf(out, "| %-6s | %-20s | %-20s | %-5s |\n", "ID", "First name", "Last name", "Grade");
    printLine(out);
}

void printStudent(FILE *out, Student s)
{
    fprintf(out, "| %-6d | %-20s | %-20s | %5.2f |\n", s.id, s.firstName, s.lastName, s.grade);
}

void printInorder(FILE *out, Node *root)
{
    if (root == NULL)
        return;
    printInorder(out, root->left);
    printStudent(out, root->data);
    printInorder(out, root->right);
}

void printTree(Node *root, int level)
{
    if (root == NULL)
        return;
    printTree(root->right, level + 1);
    for (int i = 0; i < level; i++)
        printf("        ");
    printf("%d(h=%d)\n", root->data.id, root->height);
    printTree(root->left, level + 1);
}

int sameText(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

int nameMatches(Student s, const char *name)
{
    char full[2 * NAME_LEN + 1];
    sprintf(full, "%s %s", s.firstName, s.lastName);
    return sameText(s.firstName, name) || sameText(s.lastName, name) || sameText(full, name);
}

void searchByName(Node *root, const char *name, int *count, Node **found)
{
    if (root == NULL)
        return;
    searchByName(root->left, name, count, found);
    if (nameMatches(root->data, name)) {
        if (*count == 0)
            printHeader(stdout);
        printStudent(stdout, root->data);
        *found = root;
        (*count)++;
    }
    searchByName(root->right, name, count, found);
}

void filterByGrade(Node *root, double n, int *count)
{
    if (root == NULL)
        return;
    filterByGrade(root->left, n, count);
    if (root->data.grade > n) {
        if (*count == 0)
            printHeader(stdout);
        printStudent(stdout, root->data);
        (*count)++;
    }
    filterByGrade(root->right, n, count);
}

void collectStats(Node *root, double *sum, Node **best, Node **worst)
{
    if (root == NULL)
        return;
    *sum += root->data.grade;
    if (*best == NULL || root->data.grade > (*best)->data.grade)
        *best = root;
    if (*worst == NULL || root->data.grade < (*worst)->data.grade)
        *worst = root;
    collectStats(root->left, sum, best, worst);
    collectStats(root->right, sum, best, worst);
}

void printWithGrade(Node *root, double grade)
{
    if (root == NULL)
        return;
    printWithGrade(root->left, grade);
    if (root->data.grade == grade)
        printStudent(stdout, root->data);
    printWithGrade(root->right, grade);
}

int validName(const char *s)
{
    if (s[0] == '\0')
        return 0;
    for (int i = 0; s[i] != '\0'; i++)
        if (!isalpha((unsigned char)s[i]) && s[i] != '-')
            return 0;
    return 1;
}

int validGrade(double g)
{
    return g >= 0 && g <= 10;
}

Node *loadFromFile(const char *fileName, Node *root)
{
    FILE *f = fopen(fileName, "r");
    char line[LINE_LEN];
    int lineNo = 0, loaded = 0;

    if (f == NULL) {
        printf("Cannot open \"%s\". Starting with an empty list.\n", fileName);
        return root;
    }

    while (fgets(line, sizeof(line), f) != NULL) {
        Student s;
        lineNo++;
        if (line[0] == '\n' || line[0] == '\r')
            continue;

        if (sscanf(line, "%d %49s %49s %lf", &s.id, s.firstName, s.lastName, &s.grade) != 4 ||
            s.id <= 0 || !validGrade(s.grade) || !validName(s.firstName) || !validName(s.lastName)) {
            printf("Line %d is not valid, skipped.\n", lineNo);
            continue;
        }
        if (searchById(root, s.id) != NULL) {
            printf("Line %d: ID %d already exists, skipped.\n", lineNo, s.id);
            continue;
        }
        root = insert(root, s);
        loaded++;
    }

    fclose(f);
    printf("Loaded %d student(s) from \"%s\".\n", loaded, fileName);
    return root;
}

void writeInorder(Node *root, FILE *f)
{
    if (root == NULL)
        return;
    writeInorder(root->left, f);
    fprintf(f, "%d %s %s %.2f\n", root->data.id, root->data.firstName,
            root->data.lastName, root->data.grade);
    writeInorder(root->right, f);
}

void saveToFile(const char *fileName, Node *root)
{
    FILE *f = fopen(fileName, "w");
    if (f == NULL) {
        printf("Cannot write to \"%s\".\n", fileName);
        return;
    }
    writeInorder(root, f);
    fclose(f);
    printf("File \"%s\" updated (%d student(s)).\n", fileName, countNodes(root));
}

void writeReport(const char *reportName, Node *root)
{
    FILE *f = fopen(reportName, "w");
    if (f == NULL) {
        printf("Cannot write to \"%s\".\n", reportName);
        return;
    }
    fprintf(f, "STUDENT LIST (sorted by ID)\n");
    printHeader(f);
    printInorder(f, root);
    printLine(f);
    fprintf(f, "Total: %d student(s)\n", countNodes(root));
    fclose(f);
    printf("Student list saved to \"%s\".\n", reportName);
}

int readLine(const char *text, char *buf)
{
    printf("%s", text);
    if (fgets(buf, LINE_LEN, stdin) == NULL)
        return 0;
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

int readInt(const char *text, int *value)
{
    char buf[LINE_LEN], extra;
    while (readLine(text, buf)) {
        if (sscanf(buf, "%d %c", value, &extra) == 1)
            return 1;
        printf("Please enter an integer number.\n");
    }
    return 0;
}

int readDouble(const char *text, double *value)
{
    char buf[LINE_LEN], extra;
    while (readLine(text, buf)) {
        if (sscanf(buf, "%lf %c", value, &extra) == 1)
            return 1;
        printf("Please enter a number (example: 7.5).\n");
    }
    return 0;
}

int readName(const char *text, char *name)
{
    char buf[LINE_LEN];
    while (readLine(text, buf)) {
        if (strlen(buf) < NAME_LEN && validName(buf)) {
            strcpy(name, buf);
            return 1;
        }
        printf("Name must be one word and contain only letters.\n");
    }
    return 0;
}

void menuSearchById(Node *root)
{
    int id;
    if (!readInt("Enter student ID: ", &id))
        return;
    Node *n = searchById(root, id);
    if (n == NULL) {
        printf("Student with ID %d not found.\n", id);
        return;
    }
    printHeader(stdout);
    printStudent(stdout, n->data);
    printLine(stdout);
}

void menuSearchByName(Node *root)
{
    char name[LINE_LEN];
    int count = 0;
    Node *found = NULL;
    if (!readLine("Enter name (first, last or \"first last\"): ", name))
        return;
    searchByName(root, name, &count, &found);
    if (count == 0) {
        printf("No student with name \"%s\".\n", name);
        return;
    }
    printLine(stdout);
    printf("%d student(s) found.\n", count);
}

void menuPrintAll(Node *root)
{
    if (root == NULL) {
        printf("The list is empty.\n");
        return;
    }
    printHeader(stdout);
    printInorder(stdout, root);
    printLine(stdout);
    printf("Total: %d student(s)\n", countNodes(root));
}

void menuFilter(Node *root)
{
    double n;
    int count = 0;
    if (!readDouble("Enter N (students with grade > N will be shown): ", &n))
        return;
    filterByGrade(root, n, &count);
    if (count == 0) {
        printf("No student has grade > %.2f\n", n);
        return;
    }
    printLine(stdout);
    printf("%d student(s) have grade > %.2f\n", count, n);
}

void menuStatistics(Node *root)
{
    double sum = 0;
    Node *best = NULL, *worst = NULL;

    if (root == NULL) {
        printf("The list is empty.\n");
        return;
    }
    collectStats(root, &sum, &best, &worst);

    printf("Number of students : %d\n", countNodes(root));
    printf("Average grade      : %.2f\n", sum / countNodes(root));

    printf("\nHighest grade (%.2f):\n", best->data.grade);
    printHeader(stdout);
    printWithGrade(root, best->data.grade);
    printLine(stdout);

    printf("\nLowest grade (%.2f):\n", worst->data.grade);
    printHeader(stdout);
    printWithGrade(root, worst->data.grade);
    printLine(stdout);
}

Node *menuAdd(Node *root, const char *fileName)
{
    Student s;

    if (!readInt("New student ID: ", &s.id))
        return root;
    if (s.id <= 0) {
        printf("ID must be a positive number.\n");
        return root;
    }
    if (searchById(root, s.id) != NULL) {
        printf("ID %d already exists.\n", s.id);
        return root;
    }
    if (!readName("First name: ", s.firstName) ||
        !readName("Last name: ", s.lastName) ||
        !readDouble("Grade (0-10): ", &s.grade))
        return root;
    if (!validGrade(s.grade)) {
        printf("Grade must be between 0 and 10.\n");
        return root;
    }

    root = insert(root, s);
    printf("Student %s %s added.\n", s.firstName, s.lastName);
    saveToFile(fileName, root);
    return root;
}

Node *removeStudent(Node *root, Node *n, const char *fileName)
{
    printf("Removing %s %s (ID %d)\n", n->data.firstName, n->data.lastName, n->data.id);
    root = deleteNode(root, n->data.id);
    saveToFile(fileName, root);
    return root;
}

Node *menuRemoveById(Node *root, const char *fileName)
{
    int id;
    if (!readInt("ID of the student to remove: ", &id))
        return root;
    Node *n = searchById(root, id);
    if (n == NULL) {
        printf("Student with ID %d not found.\n", id);
        return root;
    }
    return removeStudent(root, n, fileName);
}

Node *menuRemoveByName(Node *root, const char *fileName)
{
    char name[LINE_LEN];
    int count = 0, id;
    Node *found = NULL;

    if (!readLine("Name of the student to remove: ", name))
        return root;
    searchByName(root, name, &count, &found);
    if (count == 0) {
        printf("No student with name \"%s\".\n", name);
        return root;
    }
    printLine(stdout);

    if (count > 1) {
        printf("%d students have this name.\n", count);
        if (!readInt("Enter the ID of the one to remove: ", &id))
            return root;
        found = searchById(root, id);
        if (found == NULL || !nameMatches(found->data, name)) {
            printf("ID %d is not in the list above, nothing removed.\n", id);
            return root;
        }
    }
    return removeStudent(root, found, fileName);
}

void printMenu(void)
{
    printf("\n========== STUDENT GRADES (AVL TREE) ==========\n");
    printf(" 1. Search student by ID\n");
    printf(" 2. Search student by name\n");
    printf(" 3. Sort students by ID (in-order)\n");
    printf(" 4. Filter students with grade > N\n");
    printf(" 5. Statistics (average, highest, lowest)\n");
    printf(" 6. Print student list\n");
    printf(" 7. Add new student\n");
    printf(" 8. Remove student by ID\n");
    printf(" 9. Remove student by name\n");
    printf("10. Save (update) file\n");
    printf("11. Show AVL tree structure\n");
    printf("12. Save student list to %s\n", REPORT_FILE);
    printf(" 0. Exit\n");
    printf("===============================================\n");
}

int main(int argc, char *argv[])
{
    const char *fileName = (argc > 1) ? argv[1] : DEFAULT_FILE;
    Node *root = loadFromFile(fileName, NULL);
    int choice = -1;

    while (choice != 0) {
        printMenu();
        if (!readInt("Your choice (0-12): ", &choice))
            break;
        printf("\n");

        switch (choice) {
        case 1:
            menuSearchById(root);
            break;
        case 2:
            menuSearchByName(root);
            break;
        case 3:
            printf("Students sorted by ID (in-order traversal):\n");
            menuPrintAll(root);
            break;
        case 4:
            menuFilter(root);
            break;
        case 5:
            menuStatistics(root);
            break;
        case 6:
            menuPrintAll(root);
            break;
        case 7:
            root = menuAdd(root, fileName);
            break;
        case 8:
            root = menuRemoveById(root, fileName);
            break;
        case 9:
            root = menuRemoveByName(root, fileName);
            break;
        case 10:
            saveToFile(fileName, root);
            break;
        case 11:
            if (root == NULL)
                printf("The tree is empty.\n");
            else
                printTree(root, 0);
            break;
        case 12:
            writeReport(REPORT_FILE, root);
            break;
        case 0:
            printf("Thank you, bye!\n");
            break;
        default:
            printf("Wrong choice, try again.\n");
        }
    }

    freeTree(root);
    return 0;
}
