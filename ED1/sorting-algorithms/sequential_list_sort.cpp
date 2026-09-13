// Extended version of the sequential (array-based) list interactive program:
// adds binary search and six classic in-place/out-of-place sorting algorithms
// on top of the existing insert / remove / search / save / load operations.
//
// Translated and adapted from a Data Structures course exercise. The
// original had `int pivot` in the quicksort partition step, which silently
// truncates large `long int` ids; fixed here to `long int pivot`.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_NAME_LEN 100
#define MAX_ID_STR_LEN 15
#define MAX_LINE_LEN 128

struct Person {
    char name[MAX_NAME_LEN];
    long int id; // originally "rg" (Brazilian ID document number)
};

struct SequentialList {
    Person *data;
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

// Requires the list to already be sorted by id (ascending).
char* binarySearchPerson(const SequentialList *list, long int id, int &pos) {
    int high = list->size - 1;
    int low = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparisons++;
        movements++;
        if (list->data[mid].id == id) {
            pos = mid;
            char *name = new char[strlen(list->data[mid].name) + 1];
            movements++;
            strcpy(name, list->data[mid].name);
            movements++;
            return name;
        }
        comparisons++;
        if (list->data[mid].id < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
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

    while (fgets(line, sizeof(line), f)) {
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

// ---------------------------------------------------------------------
// Sorting algorithms (all sort by id, ascending).
// Comparison/movement counters approximate the dominant cost of each
// algorithm; they are not an exhaustive instruction-level count.
// ---------------------------------------------------------------------

void selectionSort(SequentialList *list) {
    for (int i = 0; i < list->size - 1; i++) {
        comparisons++;
        int minIdx = i;
        movements++;
        for (int j = i + 1; j < list->size; j++) {
            comparisons += 2;
            if (list->data[j].id < list->data[minIdx].id) {
                minIdx = j;
                movements++;
            }
        }
        Person temp = list->data[i];
        list->data[i] = list->data[minIdx];
        list->data[minIdx] = temp;
        movements += 3;
    }
}

void insertionSort(SequentialList *list) {
    for (int i = 1; i < list->size; i++) {
        comparisons++;
        Person key = list->data[i];
        int j = i - 1;
        movements++;
        while (j >= 0 && list->data[j].id > key.id) {
            comparisons++;
            list->data[j + 1] = list->data[j];
            j--;
            movements++;
        }
        list->data[j + 1] = key;
        movements++;
    }
}

void bubbleSort(SequentialList *list) {
    for (int i = 0; i < list->size - 1; i++) {
        int swapped = 0;
        comparisons++;
        for (int j = 0; j < list->size - i - 1; j++) {
            comparisons += 2;
            if (list->data[j].id > list->data[j + 1].id) {
                Person temp = list->data[j];
                list->data[j] = list->data[j + 1];
                list->data[j + 1] = temp;
                movements += 3;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

void shellSort(SequentialList *list) {
    for (int gap = list->size / 2; gap > 0; gap /= 2) {
        comparisons++;
        for (int i = gap; i < list->size; i++) {
            comparisons++;
            Person temp = list->data[i];
            movements++;
            int j;
            for (j = i; j >= gap && list->data[j - gap].id > temp.id; j -= gap) {
                list->data[j] = list->data[j - gap];
                comparisons++;
                movements++;
            }
            list->data[j] = temp;
            movements++;
        }
    }
}

int partition(SequentialList *list, int low, int high) {
    long int pivot = list->data[(low + high) / 2].id; // fixed: was `int pivot`, truncated large ids
    int i = low - 1;
    int j = high + 1;
    while (true) {
        comparisons++;
        do { i++; } while (list->data[i].id < pivot);
        comparisons++;
        do { j--; } while (list->data[j].id > pivot);
        comparisons++;
        if (i >= j) return j;
        comparisons++;
        swap(list->data[i], list->data[j]);
        movements += 3;
    }
}

void quickSort(SequentialList *list, int low, int high) {
    comparisons++;
    if (low < high) {
        int p = partition(list, low, high);
        quickSort(list, low, p);
        quickSort(list, p + 1, high);
    }
}

void merge(SequentialList *list, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Person *leftArr = new Person[n1];
    Person *rightArr = new Person[n2];
    comparisons += 2;

    for (int i = 0; i < n1; i++) {
        comparisons++;
        movements++;
        leftArr[i] = list->data[left + i];
    }
    for (int j = 0; j < n2; j++) {
        comparisons++;
        movements++;
        rightArr[j] = list->data[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons += 2;
        if (leftArr[i].id <= rightArr[j].id) {
            list->data[k] = leftArr[i];
            i++;
            movements++;
        } else {
            list->data[k] = rightArr[j];
            j++;
            movements++;
        }
        k++;
    }
    while (i < n1) {
        comparisons++;
        list->data[k] = leftArr[i];
        movements++;
        i++;
        k++;
    }
    while (j < n2) {
        comparisons++;
        list->data[k] = rightArr[j];
        movements++;
        j++;
        k++;
    }

    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(SequentialList *list, int left, int right) {
    comparisons++;
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(list, left, mid);
        mergeSort(list, mid + 1, right);
        merge(list, left, mid, right);
    }
}

int main() {
    int option = -1, sortOption = -1;
    SequentialList myList;
    myList.size = 0;
    myList.data = new Person[10000];

    loadFromFile(&myList, "IdName10K.txt");

    while (option != 0) {
        cout << "\n1 insert at start\n2 insert in middle\n3 insert at end\n";
        cout << "4 remove from start\n5 remove from middle\n6 remove from end\n";
        cout << "7 search\n8 show list\n9 save\n0 exit\n10 binary search\n11 sort\n";

        comparisons = 0;
        movements = 0;

        auto start = high_resolution_clock::now();
        cin >> option;

        if (option == 11) {
            cout << "\n1 Selection Sort\n2 Insertion Sort\n3 Bubble Sort\n4 Shell Sort\n5 Quick Sort\n6 Merge Sort\n";
            cin >> sortOption;
            if (sortOption == 1) selectionSort(&myList);
            else if (sortOption == 2) insertionSort(&myList);
            else if (sortOption == 3) bubbleSort(&myList);
            else if (sortOption == 4) shellSort(&myList);
            else if (sortOption == 5) quickSort(&myList, 0, myList.size - 1);
            else if (sortOption == 6) mergeSort(&myList, 0, myList.size - 1);
        } else if (option == 1) {
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
        } else if (option == 10) {
            long int id;
            cin >> id;
            int pos;
            char *name = binarySearchPerson(&myList, id, pos);
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
            saveToFile(&myList, "output.txt");
        }

        auto end = high_resolution_clock::now();
        auto elapsed = duration_cast<microseconds>(end - start);

        cout << "C(n): " << comparisons << "  M(n): " << movements << endl;
        cout << "Time: " << elapsed.count() << " us\n";
    }

    delete[] myList.data;
    return 0;
}
