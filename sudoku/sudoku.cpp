#include <algorithm>
#include <iostream>
#include <vector>
#include "sat.h"

using namespace std;

int getvar(int n, int i, int j, int d) {
    return 1 + i * n * n + j * n + d;
}

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
    vector<vector<int>> cnf;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cnf.emplace_back();
            for (int d = 0; d < n; d++) {
                cnf.back().push_back(getvar(n, i, j, d));
            }
            for (int d1 = 0; d1 < n; d1++) {
                for (int d2 = d1 + 1; d2 < n; d2++) {
                    cnf.push_back({-getvar(n, i, j, d1), -getvar(n, i, j, d2)});
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int d = 0; d < n; d++) {
            cnf.emplace_back();
            for (int j = 0; j < n; j++) {
                cnf.back().push_back(getvar(n, i, j, d));
            }
        }
    }
    for (int j = 0; j < n; j++) {
        for (int d = 0; d < n; d++) {
            cnf.emplace_back();
            for (int i = 0; i < n; i++) {
                cnf.back().push_back(getvar(n, i, j, d));
            }
        }
    }
    for (int i0 = 0; i0 < n; i0 += k) {
        for (int j0 = 0; j0 < n; j0 += l) {
            for (int d = 0; d < n; d++) {
                cnf.emplace_back();
                for (int i = i0; i < i0 + k; i++) {
                    for (int j = j0; j < j0 + l; j++) {
                        cnf.back().push_back(getvar(n, i, j, d));
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (field[i][j]) {
                cnf.push_back({getvar(n, i, j, field[i][j] - 1)});
            }
        }
    }
    vector<bool> sol(n * n * n);
    if (satisfiable(cnf, sol)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                auto start = sol.begin() + i * n * n + j * n;
                size_t d = find(start, start + n, true) - start;
                cout << d + 1 << ' ';
            }
            cout << endl;
        }
    }
}
