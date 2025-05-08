#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <ctime>

using namespace std;

void parallelBubble(int *a, int n);
void sequentialBubble(int *a, int n);
void swap(int &a, int &b);
void printArray(int *a, int n, int limit = 20);

// Parallel Bubble Sort
void parallelBubble(int *a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;
        #pragma omp parallel
        {
            #pragma omp for
            for (int j = first; j < n - 1; j += 2) {
                if (a[j] > a[j + 1]) {
                    swap(a[j], a[j + 1]);
                }
            }
        }
    }
}

// Sequential Bubble Sort
void sequentialBubble(int *a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

// Swap function
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Print array elements (limit controls how many to show)
void printArray(int *a, int n, int limit) {
    int toPrint = min(n, limit);
    for (int i = 0; i < toPrint; i++) {
        cout << a[i] << " ";
    }
    if (n > limit) {
        cout << "... (showing first " << limit << " of " << n << " elements)";
    }
    cout << endl;
}

int main() {
    int n = 100000;
    int *a = new int[n];
    int *b = new int[n];

    srand(time(0));
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 1000000;
        b[i] = a[i];
    }

    omp_set_num_threads(4);

    double start_parallel = omp_get_wtime();
    parallelBubble(a, n);
    double end_parallel = omp_get_wtime();
    double parallel_time = end_parallel - start_parallel;

    double start_sequential = omp_get_wtime();
    sequentialBubble(b, n);
    double end_sequential = omp_get_wtime();
    double sequential_time = end_sequential - start_sequential;

    cout << "\nTime taken by Parallel Bubble Sort: " << parallel_time << " seconds";
    cout << "\nFirst 20 elements after Parallel Sort: ";
    printArray(a, n);

    cout << "\nTime taken by Sequential Bubble Sort: " << sequential_time << " seconds";
    cout << "\nFirst 20 elements after Sequential Sort: ";
    printArray(b, n);

    delete[] a;
    delete[] b;
    return 0;
}
