#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Merge function
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Sequential Merge Sort
void sequentialMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort with limited depth
void parallelMergeSort(int arr[], int l, int r, int depth = 0) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth <= 3) {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m, depth + 1);

                #pragma omp section
                parallelMergeSort(arr, m + 1, r, depth + 1);
            }
        } else {
            sequentialMergeSort(arr, l, m);
            sequentialMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

void printFirst10(const int arr[]) {
    for (int i = 0; i < 30; i++) {
        cout << arr[i] << " ";
    }
    cout << "\n";
}

int main() {
    int n = 1000000;
    int *arr1 = new int[n];
    int *arr2 = new int[n];

    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr1[i] = rand();
        arr2[i] = arr1[i];
    }

    omp_set_num_threads(4);

    double start_parallel = omp_get_wtime();
    parallelMergeSort(arr1, 0, n - 1);
    double end_parallel = omp_get_wtime();

    double start_seq = omp_get_wtime();
    sequentialMergeSort(arr2, 0, n - 1);
    double end_seq = omp_get_wtime();

    cout << "\nTime taken by Parallel Merge Sort: " << (end_parallel - start_parallel) << " seconds";
    cout << "\nFirst 10 elements after Parallel Merge Sort:\n";
    printFirst10(arr1);

    cout << "\nTime taken by Sequential Merge Sort: " << (end_seq - start_seq) << " seconds";
    cout << "\nFirst 10 elements after Sequential Merge Sort:\n";
    printFirst10(arr2);

    delete[] arr1;
    delete[] arr2;
    return 0;
}
