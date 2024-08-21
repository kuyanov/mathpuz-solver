#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include "geom2d/figure.h"
#include "geom2d/isometry.h"

using namespace std;
using namespace Geom2d;

Figure figure;
vector<Figure> parts;
int k, n, s, cnt_f, cnt_is;

vector<vector<vector<int>>> is_map, is_rmap;
vector<vector<int>> iss;
vector<vector<vector<int>>> covering_iss, covering_iss_pos;

vector<int> sol_iss, sol_cols;
int cnt_calls = 0;

void recursive_search() {
    ++cnt_calls;
    int min_cell = -1;
    int min_cnt_covering = 1e9;
    for (int i = 0; i < s; i++) {
        if (sol_cols[i] != -1) continue;
        int cnt_covering = 0;
        for (int c = 0; c < k; c++) {
            if (sol_iss[c] == -1) {
                cnt_covering += (int) covering_iss[c][i].size();
            }
        }
        if (cnt_covering < min_cnt_covering) {
            min_cell = i;
            min_cnt_covering = cnt_covering;
        }
    }
    if (min_cell == -1) {
        cout << "FOUND" << endl;
        cout << n << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int id: figure.ord[i][j]) {
                    if (id == -1) cout << 0 << ' ';
                    else cout << sol_cols[id] + 1 << ' ';
                }
                cout << ' ';
            }
            cout << endl;
        }
        return;
    }
    for (int c = 0; c < k; c++) {
        if (sol_iss[c] != -1) continue;
        for (int is_id: covering_iss[c][min_cell]) {
            int is = iss[c][is_id];
            sol_iss[c] = is;
            for (int i = 0; i < parts[c].s; i++) {
                sol_cols[is_map[c][is_id][i]] = c;
            }
            vector<array<int, 3>> to_remove;
            for (int i = 0; i < parts[c].s; i++) {
                int j = is_map[c][is_id][i];
                for (int c2 = 0; c2 < k; c2++) {
                    for (int is_id2: covering_iss[c2][j]) {
                        for (int i2 = 0; i2 < parts[c2].s; i2++) {
                            int j2 = is_map[c2][is_id2][i2];
                            if (covering_iss_pos[c2][j2][is_id2] != -1) {
                                to_remove.push_back({c2, j2, is_id2});
                            }
                        }
                    }
                }
            }
            for (const auto &[c2, j2, is_id2]: to_remove) {
                int pos = covering_iss_pos[c2][j2][is_id2];
                int last = (int) covering_iss[c2][j2].size() - 1;
                if (pos != -1) {
                    covering_iss_pos[c2][j2][covering_iss[c2][j2][last]] = pos;
                    covering_iss_pos[c2][j2][covering_iss[c2][j2][pos]] = -1;
                    swap(covering_iss[c2][j2][pos], covering_iss[c2][j2][last]);
                    covering_iss[c2][j2].pop_back();
                }
            }
            recursive_search();
            for (const auto &[c2, j2, is_id2]: to_remove) {
                if (covering_iss_pos[c2][j2][is_id2] == -1) {
                    covering_iss_pos[c2][j2][is_id2] = (int) covering_iss[c2][j2].size();
                    covering_iss[c2][j2].push_back(is_id2);
                }
            }
            for (int i = 0; i < parts[c].s; i++) {
                sol_cols[is_map[c][is_id][i]] = -1;
            }
            sol_iss[c] = -1;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " LATTICE_FILENAME" << endl;
        return 1;
    }
    fstream lattice_fin(argv[1]);
    auto lattice = read_lattice(lattice_fin);
    figure = read_figure(cin, lattice);
    n = figure.n, s = figure.s;
    cnt_f = (int) lattice.fs.size();
    cnt_is = cnt_isometries(lattice, n);
    cin >> k;
    parts.resize(k);
    int total_area = 0;
    for (int i = 0; i < k; i++) {
        parts[i] = read_figure(cin, lattice);
        total_area += parts[i].s;
    }
    if (total_area != s) {
        cout << "NOT FOUND" << endl;
        return 0;
    }

    is_map.resize(k);
    is_rmap.resize(k);
    iss.resize(k);
    covering_iss.resize(k, vector<vector<int>>(s));
    covering_iss_pos.resize(k, vector<vector<int>>(s));
    for (int c = 0; c < k; c++) {
        for (int is = 0; is < cnt_is; is++) {
            auto op = decode_isometry(is, lattice, n);
            vector<int> is_map_layer(parts[c].s, -1), is_rmap_layer(s, -1);
            for (int i = 0; i < parts[c].n; i++) {
                for (int j = 0; j < parts[c].n; j++) {
                    for (int f = 0; f < cnt_f; f++) {
                        int id1 = parts[c].ord[i][j][f];
                        if (id1 == -1) continue;
                        auto [i2, j2, f2] = lattice_polygon_image(op, lattice, i, j, f);
                        if (i2 < 0 || i2 >= n || j2 < 0 || j2 >= n) continue;
                        int id2 = figure.ord[i2][j2][f2];
                        if (id2 == -1) continue;
                        is_map_layer[id1] = id2;
                        is_rmap_layer[id2] = id1;
                    }
                }
            }
            if (find(is_map_layer.begin(), is_map_layer.end(), -1) != is_map_layer.end()) {
                continue;
            }
            is_map[c].push_back(is_map_layer);
            is_rmap[c].push_back(is_rmap_layer);
            iss[c].push_back(is);
        }
        for (int i = 0; i < s; i++) {
            covering_iss_pos[c][i].resize(iss[c].size(), -1);
            for (int is_id = 0; is_id < (int) iss[c].size(); is_id++) {
                if (is_rmap[c][is_id][i] != -1) {
                    covering_iss_pos[c][i][is_id] = (int) covering_iss[c][i].size();
                    covering_iss[c][i].push_back(is_id);
                }
            }
        }
    }

    sol_iss.resize(k, -1);
    sol_cols.resize(s, -1);
    recursive_search();
    cout << cnt_calls << endl;
}
