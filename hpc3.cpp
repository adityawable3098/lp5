#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

using namespace std;

// Sequential Bubble Sort
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Odd-Even Transposition Sort using OpenMP
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2)
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
        } else {
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2)
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
        }
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

    double start = omp_get_wtime();
    bubbleSortSequential(copy1);
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort time: " << (end - start) << " seconds\n";

    start = omp_get_wtime();
    bubbleSortParallel(copy2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort time:   " << (end - start) << " seconds\n";

    bool is_equal = (copy1 == copy2);
    cout << "Results match: " << (is_equal ? "Yes" : "No") << endl;

    cout << "\nSorted Array:\n";
    for (int i = 0; i < copy1.size(); ++i)
        cout << copy1[i] << " ";
    cout << endl;

    return 0;
}

