#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include "geom2d/figure.h"
#include "geom2d/isometry.h"
#include "exact_cover.h"

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

    set<vector<int>> sets_unique;
    for (int c = 0; c < k; c++) {
        for (int is = 0; is < cnt_is; is++) {
            auto op = decode_isometry(is, lattice, n);
            vector<int> image;
            for (int i = 0; i < parts[c].n; i++) {
                for (int j = 0; j < parts[c].n; j++) {
                    for (int f = 0; f < cnt_f; f++) {
                        int id1 = parts[c].ord[i][j][f];
                        if (id1 == -1) continue;
                        auto [i2, j2, f2] = lattice_polygon_image(op, lattice, i, j, f);
                        if (i2 < 0 || i2 >= n || j2 < 0 || j2 >= n) continue;
                        int id2 = figure.ord[i2][j2][f2];
                        if (id2 == -1) continue;
                        image.push_back(id2);
                    }
                }
            }
            if (image.size() == parts[c].s) {
                image.push_back(s + c);
                sort(image.begin(), image.end());
                sets_unique.insert(image);
            }
        }
    }
    vector<vector<int>> sets(sets_unique.begin(), sets_unique.end());
    vector<bool> sol(sets.size());
    bool found = exact_cover(s + k, sets, sol);
    if (found) {
        cout << "FOUND" << endl;
        vector<int> col(s);
        for (int i = 0; i < (int) sets.size(); i++) {
            if (sol[i]) {
                int c = sets[i].back() - s;
                for (int j: sets[i]) {
                    if (j < s) col[j] = c;
                }
            }
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
