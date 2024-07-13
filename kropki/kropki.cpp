#include <algorithm>
#include <iostream>
#include <vector>
#include "sat.h"

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> hor(n, vector<int>(n - 1)), vert(n - 1, vector<int>(n));
    for (int i = 0; i < 2 * n - 1; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < n - 1; j++) {
                char c;
                cin >> c;
                hor[i / 2][j] = (c == 'b' ? 2 : c == 'w' ? 1 : 0);
            }
        } else {
            for (int j = 0; j < n; j++) {
                char c;
                cin >> c;
                vert[i / 2][j] = (c == 'b' ? 2 : c == 'w' ? 1 : 0);
            }
        }
    }
    vector<vector<int>> cnf;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cnf.emplace_back();
            for (int d = 0; d < n; d++) {
                cnf.back().push_back(1 + i * n * n + j * n + d);
            }
            for (int d1 = 0; d1 < n; d1++) {
                for (int d2 = d1 + 1; d2 < n; d2++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d1), -(1 + i * n * n + j * n + d2)});
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int d = 0; d < n; d++) {
            cnf.emplace_back();
            for (int j = 0; j < n; j++) {
                cnf.back().push_back(1 + i * n * n + j * n + d);
            }
        }
    }
    for (int j = 0; j < n; j++) {
        for (int d = 0; d < n; d++) {
            cnf.emplace_back();
            for (int i = 0; i < n; i++) {
                cnf.back().push_back(1 + i * n * n + j * n + d);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (hor[i][j] == 1) {
                for (int d = 0; d < n; d++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d)});
                    if (d > 0) cnf.back().push_back(1 + i * n * n + (j + 1) * n + d - 1);
                    if (d < n - 1) cnf.back().push_back(1 + i * n * n + (j + 1) * n + d + 1);
                }
            } else if (hor[i][j] == 2) {
                for (int d = 0; d < n; d++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d)});
                    if (d % 2 == 1) cnf.back().push_back(1 + i * n * n + (j + 1) * n + (d - 1) / 2);
                    if (2 * (d + 1) <= n) cnf.back().push_back(1 + i * n * n + (j + 1) * n + 2 * d + 1);
                }
            } else {
                for (int d = 0; d < n; d++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d)});
                    for (int d2 = 0; d2 < n; d2++) {
                        if (d2 != d - 1 && d2 != d + 1 && (d % 2 == 0 || d2 != (d - 1) / 2) && d2 != 2 * d + 1) {
                            cnf.back().push_back(1 + i * n * n + (j + 1) * n + d2);
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            if (vert[i][j] == 1) {
                for (int d = 0; d < n; d++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d)});
                    if (d > 0) cnf.back().push_back(1 + (i + 1) * n * n + j * n + d - 1);
                    if (d < n - 1) cnf.back().push_back(1 + (i + 1) * n * n + j * n + d + 1);
                }
            } else if (vert[i][j] == 2) {
                for (int d = 0; d < n; d++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d)});
                    if (d % 2 == 1) cnf.back().push_back(1 + (i + 1) * n * n + j * n + (d - 1) / 2);
                    if (2 * (d + 1) <= n) cnf.back().push_back(1 + (i + 1) * n * n + j * n + 2 * d + 1);
                }
            } else {
                for (int d = 0; d < n; d++) {
                    cnf.push_back({-(1 + i * n * n + j * n + d)});
                    for (int d2 = 0; d2 < n; d2++) {
                        if (d2 != d - 1 && d2 != d + 1 && (d % 2 == 0 || d2 != (d - 1) / 2) && d2 != 2 * d + 1) {
                            cnf.back().push_back(1 + (i + 1) * n * n + j * n + d2);
                        }
                    }
                }
            }
        }
    }
    vector<bool> sol(n * n * n);
    if (satisfiable(cnf, sol)) {
        cout << "SAT" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                auto start = sol.begin() + i * n * n + j * n;
                int d = find(start, start + n, true) - start;
                cout << d + 1 << ' ';
            }
            cout << endl;
        }
    } else {
        cout << "UNSAT" << endl;
    }
}
