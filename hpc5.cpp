#include <iostream>
#include <vector>
#include <climits>   // For INT_MAX and INT_MIN
#include <omp.h>

using namespace std;

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    long long sum = 0;

    // Parallel reduction for min, max, and sum
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        if (arr[i] < minVal) minVal = arr[i];  // Redundant due to reduction
        if (arr[i] > maxVal) maxVal = arr[i];  // Redundant due to reduction
        sum += arr[i];
    }

    double average = (double)sum / n;

    cout << "\nResults using Parallel Reduction:\n";
    cout << "Minimum: " << minVal << endl;
    cout << "Maximum: " << maxVal << endl;
    cout << "Sum:     " << sum << endl;
    cout << "Average: " << average << endl;

    return 0;
}

