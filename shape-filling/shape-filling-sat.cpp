#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "geom2d/figure.h"
#include "geom2d/isometry.h"
#include "sat.h"

using namespace std;
using namespace Geom2d;

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " LATTICE_FILENAME" << endl;
        return 1;
    }
    fstream lattice_fin(argv[1]);
    auto lattice = read_lattice(lattice_fin);
    auto figure = read_figure(cin, lattice);
    int n = figure.n, s = figure.s;
    int cnt_f = (int) lattice.fs.size();
    int cnt_is = cnt_isometries(lattice, n);
    int k;
    cin >> k;
    vector<Figure> parts(k);
    int total_area = 0;
    for (int i = 0; i < k; i++) {
        parts[i] = read_figure(cin, lattice);
        total_area += parts[i].s;
    }
    if (total_area != s) {
        cout << "NOT FOUND" << endl;
        return 0;
    }

    vector<vector<vector<int>>> pr(k);
    vector<int> num_is(k), shift_is(k + 1);
    for (int c = 0; c < k; c++) {
        for (int is = 0; is < cnt_is; is++) {
            auto op = decode_isometry(is, lattice, n);
            vector<int> layer(s, -1);
            for (int i = 0; i < parts[c].n; i++) {
                for (int j = 0; j < parts[c].n; j++) {
                    for (int f = 0; f < cnt_f; f++) {
                        int id1 = parts[c].ord[i][j][f];
                        if (id1 == -1) continue;
                        auto [i2, j2, f2] = lattice_polygon_image(op, lattice, i, j, f);
                        if (i2 < 0 || i2 >= n || j2 < 0 || j2 >= n) continue;
                        int id2 = figure.ord[i2][j2][f2];
                        if (id2 == -1) continue;
                        layer[id2] = id1;
                    }
                }
            }
            if (s - count(layer.begin(), layer.end(), -1) != parts[c].s) {
                continue;
            }
            pr[c].push_back(layer);
        }
        num_is[c] = (int) pr[c].size();
        shift_is[c + 1] = shift_is[c] + num_is[c];
    }

    vector<vector<int>> cnf;
    for (int i = 0; i < s; i++) {
        cnf.emplace_back();
        for (int j = 0; j < k; j++) {
            cnf.back().push_back(1 + i * k + j);
        }
        for (int j1 = 0; j1 < k; j1++) {
            for (int j2 = 0; j2 < j1; j2++) {
                cnf.push_back({-(1 + i * k + j1), -(1 + i * k + j2)});
            }
        }
    }
    for (int c = 0; c < k; c++) {
        cnf.emplace_back();
        for (int is = 0; is < num_is[c]; is++) {
            cnf.back().push_back(1 + s * k + shift_is[c] + is);
        }
        for (int is1 = 0; is1 < num_is[c]; is1++) {
            for (int is2 = 0; is2 < is1; is2++) {
                cnf.push_back({-(1 + s * k + shift_is[c] + is1), -(1 + s * k + shift_is[c] + is2)});
            }
        }
    }
    for (int c = 0; c < k; c++) {
        vector<vector<int>> covering_iss(s);
        for (int is = 0; is < num_is[c]; is++) {
            for (int i = 0; i < s; i++) {
                if (pr[c][is][i] == -1) {
                    cnf.push_back({-(1 + s * k + shift_is[c] + is), -(1 + i * k + c)});
                } else {
                    covering_iss[i].push_back(is);
                    cnf.push_back({-(1 + s * k + shift_is[c] + is), 1 + i * k + c});
                }
            }
        }
        for (int i = 0; i < s; i++) {
            cnf.push_back({-(1 + i * k + c)});
            for (int is: covering_iss[i]) {
                cnf.back().push_back(1 + s * k + shift_is[c] + is);
            }
        }
    }

    vector<bool> sol(s * k + shift_is[k]);
    //vector<string> params = {"--forcephase=true", "--probe=false", "--stable=2"};
    if (satisfiable(cnf, sol)) {
        cout << "FOUND" << endl;
        vector<int> col(s);
        for (int i = 0; i < s; i++) {
            int ind = find(sol.begin() + i * k, sol.begin() + (i + 1) * k, true) - sol.begin();
            col[i] = ind - i * k;
        }
        cout << n << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int id: figure.ord[i][j]) {
                    if (id == -1) cout << 0 << ' ';
                    else cout << col[id] + 1 << ' ';
                }
                cout << ' ';
            }
            cout << endl;
        }
    } else {
        cout << "NOT FOUND" << endl;
    }
}
