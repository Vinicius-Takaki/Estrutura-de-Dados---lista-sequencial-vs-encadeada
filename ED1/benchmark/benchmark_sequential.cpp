// Automated benchmark for the sequential (array-based) list.
// For each list size n, builds a list with n elements and measures
// comparisons/movements/time for: insert at start, insert at end,
// insert in middle, search (worst case, last element), remove at
// start, remove in middle, remove at end.
// Prints CSV rows to stdout: structure,operation,n,comparisons,movements,time_us

#include <cstdio>
#include <cstring>
#include <chrono>
using namespace std::chrono;

#define MAX_PEOPLE 20000
#define MAX_NAME_LEN 100

struct Person {
    char name[MAX_NAME_LEN];
    long int id;
};

struct SequentialList {
    Person data[MAX_PEOPLE];
    int size;
};

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

const char* searchPerson(const SequentialList *list, long int id, int &pos) {
    for (int i = 0; i < list->size; i++) {
        comparisons += 2;
        movements++;
        if (list->data[i].id == id) {
            pos = i;
            return list->data[i].name;
        }
    }
    pos = -1;
    return NULL;
}

void fillList(SequentialList *list, int n) {
    list->size = 0;
    char name[MAX_NAME_LEN];
    for (int i = 0; i < n; i++) {
        snprintf(name, MAX_NAME_LEN, "Person%d", i);
        strcpy(list->data[i].name, name);
        list->data[i].id = i;
    }
    list->size = n;
}

void reportRow(const char *op, int n) {
    printf("sequential,%s,%d,%d,%d\n", op, n, comparisons, movements);
}

int main() {
    static SequentialList list;
    int sizes[] = {100, 500, 1000, 2000, 5000, 10000};

    for (int n : sizes) {
        // insert at start
        fillList(&list, n);
        comparisons = 0; movements = 0;
        insertPerson(&list, "NewPerson", 999999, 0);
        reportRow("insert_start", n);

        // insert at end
        fillList(&list, n);
        comparisons = 0; movements = 0;
        insertPerson(&list, "NewPerson", 999999, list.size);
        reportRow("insert_end", n);

        // insert in middle
        fillList(&list, n);
        comparisons = 0; movements = 0;
        insertPerson(&list, "NewPerson", 999999, n / 2);
        reportRow("insert_middle", n);

        // search worst case (last element / not found)
        fillList(&list, n);
        comparisons = 0; movements = 0;
        int pos;
        searchPerson(&list, n - 1, pos);
        reportRow("search_worst", n);

        // remove at start
        fillList(&list, n);
        comparisons = 0; movements = 0;
        removePerson(&list, 0);
        reportRow("remove_start", n);

        // remove in middle
        fillList(&list, n);
        comparisons = 0; movements = 0;
        removePerson(&list, n / 2);
        reportRow("remove_middle", n);

        // remove at end
        fillList(&list, n);
        comparisons = 0; movements = 0;
        removePerson(&list, n - 1);
        reportRow("remove_end", n);
    }

    return 0;
}
