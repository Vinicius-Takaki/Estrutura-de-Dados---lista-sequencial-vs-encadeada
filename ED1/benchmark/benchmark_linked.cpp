// Automated benchmark for the linked list.
// Same methodology as benchmark_sequential.cpp so the two structures
// can be compared fairly.
// Prints CSV rows to stdout: structure,operation,n,comparisons,movements

#include <cstdio>
#include <cstring>

#define MAX_NAME_LEN 100

struct Person {
    char name[MAX_NAME_LEN];
    long int id;
};

struct Node {
    Person person;
    Node *next;
};

Node *head = NULL;
Node *tail = NULL;
int comparisons = 0, movements = 0;

void insertAtStart(const char *name, long int id) {
    Node *newNode = new Node;
    strcpy(newNode->person.name, name);
    newNode->person.id = id;
    newNode->next = head;
    movements += 3;
    if (head == NULL) tail = newNode;
    head = newNode;
    movements++;
    comparisons += 2;
}

void insertAtEnd(const char *name, long int id) {
    Node *newNode = new Node;
    strcpy(newNode->person.name, name);
    newNode->person.id = id;
    newNode->next = NULL;
    movements += 3;
    comparisons++;
    if (head == NULL) {
        head = newNode;
    } else {
        tail->next = newNode;
    }
    movements++;
    tail = newNode;
    movements++;
}

void insertAtMiddle(const char *name, long int id, int pos) {
    Node *newNode = new Node;
    strcpy(newNode->person.name, name);
    newNode->person.id = id;
    movements += 2;
    Node *current = head;
    for (int i = 0; i < pos - 1; i++) {
        current = current->next;
        movements++;
        comparisons++;
    }
    comparisons++;
    if (current != NULL) {
        newNode->next = current->next;
        current->next = newNode;
        movements += 2;
    }
}

void removeAtStart() {
    comparisons++;
    if (head != NULL) {
        Node *aux = head;
        head = head->next;
        movements++;
        comparisons++;
        if (head == NULL) tail = NULL;
        delete aux;
        movements++;
    }
}

void removeAtEnd() {
    comparisons++;
    if (head != NULL) {
        comparisons++;
        if (head->next == NULL) {
            delete head;
            head = NULL;
            tail = NULL;
            movements += 3;
        } else {
            Node *current = head;
            Node *aux = NULL;
            while (current->next != NULL) {
                aux = current;
                current = current->next;
                comparisons++;
            }
            comparisons++;
            aux->next = NULL;
            tail = aux;
            delete current;
            movements += 3;
        }
    }
}

void removeAtMiddle(int pos) {
    comparisons++;
    if (head == NULL) return;
    Node *current = head;
    Node *aux = NULL;
    for (int i = 0; i < pos; i++) {
        comparisons++;
        aux = current;
        current = current->next;
        movements += 2;
    }
    comparisons++;
    if (current != NULL) {
        comparisons++;
        if (aux != NULL) {
            aux->next = current->next;
            movements++;
        }
        delete current;
        movements++;
    }
}

char* searchById(long int id) {
    Node *current = head;
    while (current != NULL) {
        comparisons += 2;
        if (current->person.id == id) {
            return current->person.name;
        }
        current = current->next;
        movements++;
    }
    return NULL;
}

void clearList() {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        delete current;
        current = next;
    }
    head = NULL;
    tail = NULL;
}

void fillList(int n) {
    clearList();
    char name[MAX_NAME_LEN];
    for (int i = 0; i < n; i++) {
        snprintf(name, MAX_NAME_LEN, "Person%d", i);
        insertAtEnd(name, i);
    }
}

void reportRow(const char *op, int n) {
    printf("linked,%s,%d,%d,%d\n", op, n, comparisons, movements);
}

int main() {
    int sizes[] = {100, 500, 1000, 2000, 5000, 10000};

    for (int n : sizes) {
        fillList(n);
        comparisons = 0; movements = 0;
        insertAtStart("NewPerson", 999999);
        reportRow("insert_start", n);

        fillList(n);
        comparisons = 0; movements = 0;
        insertAtEnd("NewPerson", 999999);
        reportRow("insert_end", n);

        fillList(n);
        comparisons = 0; movements = 0;
        insertAtMiddle("NewPerson", 999999, n / 2);
        reportRow("insert_middle", n);

        fillList(n);
        comparisons = 0; movements = 0;
        searchById(n - 1);
        reportRow("search_worst", n);

        fillList(n);
        comparisons = 0; movements = 0;
        removeAtStart();
        reportRow("remove_start", n);

        fillList(n);
        comparisons = 0; movements = 0;
        removeAtMiddle(n / 2);
        reportRow("remove_middle", n);

        fillList(n);
        comparisons = 0; movements = 0;
        removeAtEnd();
        reportRow("remove_end", n);
    }

    return 0;
}
