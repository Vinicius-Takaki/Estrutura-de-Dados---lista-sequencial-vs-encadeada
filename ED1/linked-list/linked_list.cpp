#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_NAME_LEN 100
#define MAX_LINE_LEN 128

struct Person {
    char name[MAX_NAME_LEN];
    long int id; // originally "rg" (Brazilian ID document number)
};

struct Node {
    Person person;
    Node *next;
};

Node *head = NULL;
Node *tail = NULL;

// Global counters used to measure algorithm cost:
// comparisons (cn) and data movements/assignments (mn)
int comparisons = 0, movements = 0;

void insertAtStart(char *name, long int id) {
    comparisons++;
    movements++;
    Node *newNode = new Node;
    if (newNode != NULL) {
        comparisons++;
        strcpy(newNode->person.name, name);
        newNode->person.id = id;
        newNode->next = head;
        movements += 3;
        comparisons++;
        if (head == NULL)
            tail = newNode;

        head = newNode;
        movements++;
    }
}

void insertAtEnd(char *name, long int id) {
    Node *newNode = new Node;
    comparisons++;
    movements++;
    if (newNode != NULL) {
        comparisons++;
        strcpy(newNode->person.name, name);
        newNode->person.id = id;
        newNode->next = NULL;
        movements += 3;
        comparisons++;
        if (head == NULL) {
            head = newNode;
            movements++;
        } else {
            tail->next = newNode;
            movements++;
        }
        comparisons++;

        tail = newNode;
        movements++;
    }
}

void insertAtMiddle(char *name, long int id, int pos) {
    Node *newNode = new Node;
    movements++;
    comparisons++;
    if (newNode != NULL) {
        strcpy(newNode->person.name, name);
        newNode->person.id = id;
        movements += 2;
        Node *current = head;
        comparisons++;
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
}

Person removeFromStart() {
    Person p = {"", 0};

    comparisons++;
    if (head != NULL) {
        Node *aux = head;
        p = head->person;
        head = head->next;
        movements += 2;
        comparisons++;
        if (head == NULL) tail = NULL;
        delete aux;
        movements++;
    }

    return p;
}

Person removeFromEnd() {
    Person p = {"", 0};

    comparisons++;
    if (head != NULL) {
        comparisons++;
        if (head->next == NULL) {
            p = head->person;
            delete head;
            head = NULL;
            tail = NULL;
            movements += 4;
        } else {
            Node *current = head;
            Node *aux = NULL;

            while (current->next != NULL) {
                aux = current;
                current = current->next;
                comparisons++;
            }
            comparisons++;
            p = current->person;
            aux->next = NULL;
            tail = aux;
            delete current;
            movements += 4;
        }
    }

    return p;
}

Person removeFromMiddle(int pos) {
    Person p = {"", 0};

    comparisons++;
    if (head == NULL) return p;

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
        p = current->person;
        comparisons++;
        if (aux != NULL) {
            aux->next = current->next;
            movements++;
        }

        delete current;
        movements++;
    }

    return p;
}

char* searchById(long int id) {
    Node *current = head;

    while (current != NULL) {
        comparisons += 2;

        if (current->person.id == id) {
            char *name = new char[strlen(current->person.name) + 1];
            strcpy(name, current->person.name);
            movements += 2;
            return name;
        }

        current = current->next;
        movements++;
    }

    return NULL;
}

void saveToFile(const char *fileName) {
    FILE *f = fopen(fileName, "w");
    if (f == NULL) return;

    Node *current = head;

    while (current != NULL) {
        fprintf(f, "%s,%ld\n", current->person.name, current->person.id);
        current = current->next;
    }

    fclose(f);
}

void loadFromFile(const char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) return;

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), f)) {
        char name[MAX_NAME_LEN];
        long int id;
        sscanf(line, "%[^,],%ld", name, &id);
        insertAtEnd(name, id);
    }

    fclose(f);
}

void showList() {
    Node *current = head;

    comparisons++;
    if (current == NULL) {
        cout << "List is empty" << endl;
        return;
    }

    while (current != NULL) {
        comparisons++;
        cout << current->person.name << " - " << current->person.id << endl;
        current = current->next;
        movements++;
    }
}

int main() {
    int option = -1;

    loadFromFile("IdName10.txt");

    while (option != 0) {
        cout << "\n1 insert at start\n2 insert in middle\n3 insert at end\n";
        cout << "4 remove from start\n5 remove from middle\n6 remove from end\n";
        cout << "7 search by id\n8 show list\n9 save\n0 exit\n";

        cin >> option;

        comparisons = 0;
        movements = 0;
        auto start = high_resolution_clock::now();

        if (option == 1) {
            char name[MAX_NAME_LEN];
            long int id;
            cin >> name;
            cin >> id;
            insertAtStart(name, id);

        } else if (option == 2) {
            char name[MAX_NAME_LEN];
            long int id;
            int pos;
            cin >> name;
            cin >> id;
            cin >> pos;
            insertAtMiddle(name, id, pos);

        } else if (option == 3) {
            char name[MAX_NAME_LEN];
            long int id;
            cin >> name;
            cin >> id;
            insertAtEnd(name, id);

        } else if (option == 4) {
            Person p = removeFromStart();
            cout << p.name << " - " << p.id << endl;

        } else if (option == 5) {
            int pos;
            cin >> pos;
            Person p = removeFromMiddle(pos);
            cout << p.name << " - " << p.id << endl;

        } else if (option == 6) {
            Person p = removeFromEnd();
            cout << p.name << " - " << p.id << endl;

        } else if (option == 7) {
            long int id;
            cin >> id;

            char *name = searchById(id);

            if (name != NULL) {
                cout << name << endl;
                delete[] name;
            } else {
                cout << "Not found\n";
            }

        } else if (option == 8) {
            showList();

        } else if (option == 9) {
            saveToFile("IdName10.txt");
        }

        auto end = high_resolution_clock::now();
        auto elapsed = duration_cast<microseconds>(end - start);

        cout << "C(n): " << comparisons << "  M(n): " << movements << endl;
        cout << "Time: " << elapsed.count() << " us\n";
    }

    return 0;
}
