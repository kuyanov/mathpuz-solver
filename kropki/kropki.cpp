#include <algorithm>
#include <iostream>
#include <vector>
#include "sat.h"

using namespace std;

int getvar(int n, int i, int j, int d) {
    return 1 + i * n * n + j * n + d;
}

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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            for (int d = 0; d < n; d++) {
                int v0 = getvar(n, i, j, d);
                int v1 = d > 0 ? getvar(n, i, j + 1, d - 1) : -1;
                int v2 = d < n - 1 ? getvar(n, i, j + 1, d + 1) : -1;
                int v3 = d % 2 == 1 ? getvar(n, i, j + 1, (d - 1) / 2) : -1;
                int v4 = 2 * (d + 1) <= n ? getvar(n, i, j + 1, 2 * d + 1) : -1;
                if (hor[i][j] == 1) {
                    cnf.push_back({-v0});
                    if (v1 != -1) cnf.back().push_back(v1);
                    if (v2 != -1) cnf.back().push_back(v2);
                } else if (hor[i][j] == 2) {
                    cnf.push_back({-v0});
                    if (v3 != -1) cnf.back().push_back(v3);
                    if (v4 != -1) cnf.back().push_back(v4);
                } else {
                    if (v1 != -1) cnf.push_back({-v0, -v1});
                    if (v2 != -1) cnf.push_back({-v0, -v2});
                    if (v3 != -1) cnf.push_back({-v0, -v3});
                    if (v4 != -1) cnf.push_back({-v0, -v4});
                }
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < n; d++) {
                int v0 = getvar(n, i, j, d);
                int v1 = d > 0 ? getvar(n, i + 1, j, d - 1) : -1;
                int v2 = d < n - 1 ? getvar(n, i + 1, j, d + 1) : -1;
                int v3 = d % 2 == 1 ? getvar(n, i + 1, j, (d - 1) / 2) : -1;
                int v4 = 2 * (d + 1) <= n ? getvar(n, i + 1, j, 2 * d + 1) : -1;
                if (vert[i][j] == 1) {
                    cnf.push_back({-v0});
                    if (v1 != -1) cnf.back().push_back(v1);
                    if (v2 != -1) cnf.back().push_back(v2);
                } else if (vert[i][j] == 2) {
                    cnf.push_back({-v0});
                    if (v3 != -1) cnf.back().push_back(v3);
                    if (v4 != -1) cnf.back().push_back(v4);
                } else {
                    if (v1 != -1) cnf.push_back({-v0, -v1});
                    if (v2 != -1) cnf.push_back({-v0, -v2});
                    if (v3 != -1) cnf.push_back({-v0, -v3});
                    if (v4 != -1) cnf.push_back({-v0, -v4});
                }
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
