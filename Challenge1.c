// Approach to Computing Travel Costs Between Cities

// 1. Direct Costs: Use the distances d[i] between adjacent cities Ci and Ci+1 to set base costs.

// 2. Cummulative Distance Array: Computed an array cummulative_dist where each entry holds the cumulative distance from the first city C1 to city Ci.

// 3. Cost Calculation: For each city pair (i, j) with i < j, used cummulative_dist to calculate travel cost as cummulative_dist[j] - cummulative_dist[i].

// 4. Fill Cost Table: Filled 2D cost table with these computed costs for all pairs of cities.

// 5. Efficiency: This method ensures an O(n^2) time complexity by avoiding redundant calculations.


#include <stdio.h>
#include <stdlib.h>

// Function to compute travel costs using precomputed distances
void compute_travel_costs(int *d, int n, int **costs) {
    int *cummulative_dist = (int *)malloc(n * sizeof(int));
    cummulative_dist[0] = 0; // No distance to the first city

    // Compute cummulative distances
    for (int i = 1; i < n; ++i) {
        cummulative_dist[i] = cummulative_dist[i - 1] + d[i - 1];
    }

    // Fill in the cost table using cummulative distances
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            costs[i][j] = cummulative_dist[j] - cummulative_dist[i];
            costs[j][i] = costs[i][j];
        }
    }

    free(cummulative_dist);
}

int main() {
    // Example test case
    int d[] = {5, 10, 20, 10}; // Example distances between adjacent cities
    int n = sizeof(d) / sizeof(d[0]) + 1; // Number of cities
    int **costs = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        costs[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            costs[i][j] = 0;
        }
    }

    compute_travel_costs(d, n, costs);

    // Print the cost table
    printf("Cost Table:\n");
    for(int i=1;i<=n;i++){
        printf("C%i",i);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%4d ", costs[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < n; ++i) {
        free(costs[i]);
    }
    free(costs);

    return 0;
}
