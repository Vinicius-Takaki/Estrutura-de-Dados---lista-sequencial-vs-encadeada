// Automated benchmark for the six sorting algorithms implemented in
// sorting-algorithms/sequential_list_sort.cpp.
// For each list size and each initial order (random / sorted / reverse),
// runs every algorithm once on a fresh copy of the data and reports the
// comparisons and movements it took.
// Prints CSV rows to stdout: algorithm,order,n,comparisons,movements

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

#define MAX_NAME_LEN 100

struct Person {
    char name[MAX_NAME_LEN];
    long int id;
};

int comparisons = 0, movements = 0;

void selectionSort(Person *data, int size) {
    for (int i = 0; i < size - 1; i++) {
        comparisons++;
        int minIdx = i;
        movements++;
        for (int j = i + 1; j < size; j++) {
            comparisons += 2;
            if (data[j].id < data[minIdx].id) {
                minIdx = j;
                movements++;
            }
        }
        Person temp = data[i];
        data[i] = data[minIdx];
        data[minIdx] = temp;
        movements += 3;
    }
}

void insertionSort(Person *data, int size) {
    for (int i = 1; i < size; i++) {
        comparisons++;
        Person key = data[i];
        int j = i - 1;
        movements++;
        while (j >= 0 && data[j].id > key.id) {
            comparisons++;
            data[j + 1] = data[j];
            j--;
            movements++;
        }
        data[j + 1] = key;
        movements++;
    }
}

void bubbleSort(Person *data, int size) {
    for (int i = 0; i < size - 1; i++) {
        int swapped = 0;
        comparisons++;
        for (int j = 0; j < size - i - 1; j++) {
            comparisons += 2;
            if (data[j].id > data[j + 1].id) {
                Person temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
                movements += 3;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

void shellSort(Person *data, int size) {
    for (int gap = size / 2; gap > 0; gap /= 2) {
        comparisons++;
        for (int i = gap; i < size; i++) {
            comparisons++;
            Person temp = data[i];
            movements++;
            int j;
            for (j = i; j >= gap && data[j - gap].id > temp.id; j -= gap) {
                data[j] = data[j - gap];
                comparisons++;
                movements++;
            }
            data[j] = temp;
            movements++;
        }
    }
}

int partition(Person *data, int low, int high) {
    long int pivot = data[(low + high) / 2].id;
    int i = low - 1;
    int j = high + 1;
    while (true) {
        comparisons++;
        do { i++; } while (data[i].id < pivot);
        comparisons++;
        do { j--; } while (data[j].id > pivot);
        comparisons++;
        if (i >= j) return j;
        comparisons++;
        swap(data[i], data[j]);
        movements += 3;
    }
}

void quickSort(Person *data, int low, int high) {
    comparisons++;
    if (low < high) {
        int p = partition(data, low, high);
        quickSort(data, low, p);
        quickSort(data, p + 1, high);
    }
}

void merge(Person *data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Person *leftArr = new Person[n1];
    Person *rightArr = new Person[n2];
    comparisons += 2;

    for (int i = 0; i < n1; i++) {
        comparisons++;
        movements++;
        leftArr[i] = data[left + i];
    }
    for (int j = 0; j < n2; j++) {
        comparisons++;
        movements++;
        rightArr[j] = data[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons += 2;
        if (leftArr[i].id <= rightArr[j].id) {
            data[k] = leftArr[i];
            i++;
            movements++;
        } else {
            data[k] = rightArr[j];
            j++;
            movements++;
        }
        k++;
    }
    while (i < n1) {
        comparisons++;
        data[k] = leftArr[i];
        movements++;
        i++;
        k++;
    }
    while (j < n2) {
        comparisons++;
        data[k] = rightArr[j];
        movements++;
        j++;
        k++;
    }

    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(Person *data, int left, int right) {
    comparisons++;
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

void fillRandom(Person *data, int n) {
    for (int i = 0; i < n; i++) {
        snprintf(data[i].name, MAX_NAME_LEN, "Person%d", i);
        data[i].id = i;
    }
    for (int i = n - 1; i > 0; i--) { // Fisher-Yates shuffle
        int j = rand() % (i + 1);
        swap(data[i], data[j]);
    }
}

void fillSorted(Person *data, int n) {
    for (int i = 0; i < n; i++) {
        snprintf(data[i].name, MAX_NAME_LEN, "Person%d", i);
        data[i].id = i;
    }
}

void fillReverse(Person *data, int n) {
    for (int i = 0; i < n; i++) {
        snprintf(data[i].name, MAX_NAME_LEN, "Person%d", i);
        data[i].id = n - 1 - i;
    }
}

void reportRow(const char *algorithm, const char *order, int n) {
    printf("%s,%s,%d,%d,%d\n", algorithm, order, n, comparisons, movements);
}

int main() {
    printf("algorithm,order,n,comparisons,movements\n");
    srand(42); // fixed seed so results_sorting.csv is reproducible across runs
    int sizes[] = {100, 500, 1000, 2000, 5000, 10000};
    const char *orders[] = {"random", "sorted", "reverse"};

    for (int n : sizes) {
        for (const char *order : orders) {
            Person *base = new Person[n];
            if (strcmp(order, "random") == 0) fillRandom(base, n);
            else if (strcmp(order, "sorted") == 0) fillSorted(base, n);
            else fillReverse(base, n);

            Person *work = new Person[n];

            memcpy(work, base, n * sizeof(Person));
            comparisons = 0; movements = 0;
            selectionSort(work, n);
            reportRow("selection", order, n);

            memcpy(work, base, n * sizeof(Person));
            comparisons = 0; movements = 0;
            insertionSort(work, n);
            reportRow("insertion", order, n);

            memcpy(work, base, n * sizeof(Person));
            comparisons = 0; movements = 0;
            bubbleSort(work, n);
            reportRow("bubble", order, n);

            memcpy(work, base, n * sizeof(Person));
            comparisons = 0; movements = 0;
            shellSort(work, n);
            reportRow("shell", order, n);

            memcpy(work, base, n * sizeof(Person));
            comparisons = 0; movements = 0;
            quickSort(work, 0, n - 1);
            reportRow("quick", order, n);

            memcpy(work, base, n * sizeof(Person));
            comparisons = 0; movements = 0;
            mergeSort(work, 0, n - 1);
            reportRow("merge", order, n);

            delete[] base;
            delete[] work;
        }
    }

    return 0;
}
