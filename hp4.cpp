#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

using namespace std;

// Merge two sorted halves
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j])
            arr[k++] = leftArr[i++];
        else
            arr[k++] = rightArr[j++];
    }
    while (i < leftArr.size())
        arr[k++] = leftArr[i++];
    while (j < rightArr.size())
        arr[k++] = rightArr[j++];
}

// Sequential Merge Sort
void mergeSortSequential(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort using OpenMP
void mergeSortParallel(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = (left + right) / 2;

        if (depth < 4) { // Limit depth to avoid thread oversubscription
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(arr, left, mid, depth + 1);

                #pragma omp section
                mergeSortParallel(arr, mid + 1, right, depth + 1);
            }
        } else {
            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> original(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; ++i)
        cin >> original[i];

    vector<int> copy1 = original;
    vector<int> copy2 = original;

    // Sequential
    double start = omp_get_wtime();
    mergeSortSequential(copy1, 0, n - 1);
    double end = omp_get_wtime();
    cout << "Sequential Merge Sort time: " << (end - start) << " seconds\n";

    // Parallel
    start = omp_get_wtime();
    mergeSortParallel(copy2, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort time:   " << (end - start) << " seconds\n";

    bool is_equal = (copy1 == copy2);
    cout << "Results match: " << (is_equal ? "Yes" : "No") << endl;

    cout << "\nSorted Array:\n";
    for (int i = 0; i < copy1.size(); ++i)
        cout << copy1[i] << " ";
    cout << endl;

    return 0;
}

