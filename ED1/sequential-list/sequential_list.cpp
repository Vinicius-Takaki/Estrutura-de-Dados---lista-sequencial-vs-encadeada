#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_PEOPLE 50
#define MAX_NAME_LEN 100
#define MAX_ID_STR_LEN 15
#define MAX_LINE_LEN 128

struct Person {
    char name[MAX_NAME_LEN];
    long int id; // originally "rg" (Brazilian ID document number)
};

struct SequentialList {
    Person data[MAX_PEOPLE];
    int size;
};

// Global counters used to measure algorithm cost:
// comparisons (cn) and data movements/assignments (mn)
int comparisons = 0, movements = 0;

void insertPerson(SequentialList *list, const char *name, long int id, int pos) {
    for (int i = list->size; i > pos; i--) {
        comparisons++;
        movements++;
        list->data[i] = list->data[i - 1];
    }

    strcpy(list->data[pos].name, name);
    list->data[pos].id = id;
    list->size++;
    movements += 3;
}

void removePerson(SequentialList *list, int pos) {
    for (int i = pos; i < list->size - 1; i++) {
        comparisons++;
        movements++;
        list->data[i] = list->data[i + 1];
    }
    list->size--;
    movements++;
}

char* searchPerson(const SequentialList *list, long int id, int &pos) {
    for (int i = 0; i < list->size; i++) {
        comparisons += 2;
        movements++;
        if (list->data[i].id == id) {
            pos = i;
            char *name = new char[strlen(list->data[i].name) + 1];
            movements++;
            strcpy(name, list->data[i].name);
            movements++;
            return name;
        }
    }

    pos = -1;
    return NULL;
}

void saveToFile(const SequentialList *list, const char *fileName) {
    FILE *f = fopen(fileName, "w");
    comparisons++;
    if (f == NULL) return;

    for (int i = 0; i < list->size; i++) {
        fprintf(f, "%s,%ld\n", list->data[i].name, list->data[i].id);
    }

    fclose(f);
}

void loadFromFile(SequentialList *list, const char *fileName) {
    FILE *f = fopen(fileName, "r");
    comparisons++;
    if (f == NULL) return;

    char line[MAX_LINE_LEN];
    int count = 0;

    while (count < MAX_PEOPLE && fgets(line, sizeof(line), f)) {
        comparisons++;
        char tempName[MAX_NAME_LEN];
        char tempIdStr[MAX_ID_STR_LEN];

        comparisons++;
        if (sscanf(line, "%[^,],%s", tempName, tempIdStr) == 2) {
            strcpy(list->data[count].name, tempName);
            list->data[count].id = atol(tempIdStr);
            movements += 3;
            count++;
        }
    }

    fclose(f);
    list->size = count;
    movements++;
}

void showList(const SequentialList *list) {
    comparisons++;
    if (list->size == 0) {
        printf("List is empty\n");
        return;
    }

    for (int i = 0; i < list->size; i++) {
        comparisons++;
        printf("%s - %ld\n", list->data[i].name, list->data[i].id);
    }
}

int main() {
    int option = -1;
    SequentialList myList;
    myList.size = 0;

    loadFromFile(&myList, "IdName10.txt");

    while (option != 0) {
        cout << "\n1 insert at start\n2 insert in middle\n3 insert at end\n";
        cout << "4 remove from start\n5 remove from middle\n6 remove from end\n";
        cout << "7 search\n8 show list\n9 save\n0 exit\n";

        cin >> option;

        comparisons = 0;
        movements = 0;

        auto start = high_resolution_clock::now();

        if (option == 1) {
            char name[MAX_NAME_LEN];
            long int id;

            cin >> name >> id;

            insertPerson(&myList, name, id, 0);

            cout << "Position: 0\n";
        } else if (option == 2) {
            char name[MAX_NAME_LEN];
            long int id;
            int pos;

            cin >> name >> id >> pos;

            insertPerson(&myList, name, id, pos);

            cout << "Position: " << pos << endl;
        } else if (option == 3) {
            char name[MAX_NAME_LEN];
            long int id;

            cin >> name >> id;

            insertPerson(&myList, name, id, myList.size);

            cout << "Position: " << myList.size - 1 << endl;
        } else if (option == 4) {
            removePerson(&myList, 0);

            cout << "Position: 0\n";
        } else if (option == 5) {
            int pos;

            cin >> pos;

            removePerson(&myList, pos);

            cout << "Position: " << pos << endl;
        } else if (option == 6) {
            int pos = myList.size - 1;

            removePerson(&myList, pos);

            cout << "Position: " << pos << endl;
        } else if (option == 7) {
            long int id;
            cin >> id;

            int pos;
            char *name = searchPerson(&myList, id, pos);

            if (name != NULL) {
                cout << name << endl;
                cout << "Position: " << pos << endl;
                delete[] name;
            } else {
                cout << "Not found\n";
            }
        } else if (option == 8) {
            showList(&myList);
        } else if (option == 9) {
            saveToFile(&myList, "IdName10.txt");
        }

        auto end = high_resolution_clock::now();
        auto elapsed = duration_cast<microseconds>(end - start);

        cout << "C(n): " << comparisons << "  M(n): " << movements << endl;
        cout << "Time: " << elapsed.count() << " us\n";
    }

    return 0;
}
