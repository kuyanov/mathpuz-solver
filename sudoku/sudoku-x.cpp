#include <iostream>
#include <vector>
#include "exact_cover.h"

using namespace std;

int main() {
    int k, l;
    cin >> k >> l;
    int n = k * l;
    vector<vector<int>> field(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> field[i][j];
        }
    }
    int cnt_sets = n * n * n, cnt_vs = 4 * n * n;
    vector<vector<int>> sets(cnt_sets);
    vector<bool> sol(cnt_sets);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (field[i][j]) {
                int r = n * n * i + n * j + field[i][j] - 1;
                sol[r] = true;
            }
            int i0 = i / k, j0 = j / l;
            for (int d = 0; d < n; d++) {
                int r = n * n * i + n * j + d;
                sets[r] = {n * i + j, n * n + n * i + d, 2 * n * n + n * j + d,
                           3 * n * n + n * k * i0 + n * j0 + d};
            }
        }
    }
    bool found = exact_cover(cnt_vs, sets, sol, false);
    if (found) {
        cout << "FOUND" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int d = 0; d < n; d++) {
                    if (sol[n * n * i + n * j + d]) {
                        cout << d + 1 << ' ';
                        break;
                    }
                }
            }
            cout << endl;
        }
    } else {
        cout << "NOT FOUND" << endl;
    }
}
