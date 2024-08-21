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
    if (s % k != 0) {
        cout << "NOT FOUND" << endl;
        return 0;
    }

    vector<vector<int>> g, rg;
    for (int is = 0; is < cnt_is; is++) {
        auto op = decode_isometry(is, lattice, n);
        vector<int> g_layer(s, -1), rg_layer(s, -1);
        int cnt_edges = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int f = 0; f < cnt_f; f++) {
                    int id1 = figure.ord[i][j][f];
                    if (id1 == -1) continue;
                    auto [i2, j2, f2] = lattice_polygon_image(op, lattice, i, j, f);
                    if (i2 < 0 || i2 >= n || j2 < 0 || j2 >= n) continue;
                    int id2 = figure.ord[i2][j2][f2];
                    if (id2 == -1) continue;
                    g_layer[id1] = id2;
                    rg_layer[id2] = id1;
                    ++cnt_edges;
                }
            }
        }
        if (cnt_edges >= s / k) {
            g.push_back(std::move(g_layer));
            rg.push_back(std::move(rg_layer));
        }
    }
    cnt_is = (int) g.size();

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
    for (int c = 0; c < k - 1; c++) {
        cnf.emplace_back();
        for (int is = 0; is < cnt_is; is++) {
            cnf.back().push_back(1 + s * k + c * cnt_is + is);
        }
    }
    for (int is = 0; is < cnt_is; is++) {
        for (int c = 0; c < k - 1; c++) {
            int id_is = 1 + s * k + c * cnt_is + is;
            for (int i = 0; i < s; i++) {
                if (g[is][i] == -1) {
                    cnf.push_back({-(1 + i * k + (k - 1)), -id_is});
                } else {
                    cnf.push_back({-(1 + i * k + (k - 1)), -id_is, 1 + g[is][i] * k + c});
                }
                if (rg[is][i] == -1) {
                    cnf.push_back({-(1 + i * k + c), -id_is});
                } else {
                    cnf.push_back({-(1 + i * k + c), -id_is, 1 + rg[is][i] * k + (k - 1)});
                }
            }
        }
    }
    if (cnt_f == 12) {
        for (int i = 0; i < s; i += 6) {
            for (int c = 0; c < k; c++) {
                vector<int> xs(6);
                for (int num = 0; num < 6; num++) {
                    xs[num] = 1 + (i + num) * k + c;
                }
                for (int num = 0; num < 6; num++) {
                    cnf.push_back({-xs[num], xs[(num + 3) % 6], xs[(num + 2) % 6]});
                    cnf.push_back({-xs[num], xs[(num + 3) % 6], xs[(num + 4) % 6]});
                }
            }
        }
    }
    cnf.push_back({1 + 0 * k + (k - 1)});
//    for (int c = 0; c < k - 2; c++) {
//        for (int is1 = 0; is1 < cnt_is; is1++) {
//            for (int is2 = 0; is2 < is1; is2++) {
//                cnf.push_back({-(1 + s * k + c * cnt_is + is1), -(1 + s * k + (c + 1) * cnt_is + is2)});
//            }
//        }
//    }

    vector<bool> sol(s * k + (k - 1) * cnt_is);
    while (satisfiable(cnf, sol)) {
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

        for (const auto &layer: g) {
            vector<int> target;
            for (int i = 0; i < s; i++) {
                if (col[i] == k - 1) {
                    target.push_back(layer[i]);
                }
            }
            if (find(target.begin(), target.end(), -1) != target.end()) {
                continue;
            }
            cnf.emplace_back();
            for (int j: target) {
                cnf.back().push_back(-(1 + j * k + (k - 1)));
            }
        }
    }
    cout << "NOT FOUND" << endl;
}
