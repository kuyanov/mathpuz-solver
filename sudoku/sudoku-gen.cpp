#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

bool check(int k, int l, const vector<vector<int>> &field) {
    int n = k * l;
    for (int i = 0; i < n; i++) {
        vector<int> cnt(n);
        for (int j = 0; j < n; j++) {
            if (field[i][j]) cnt[field[i][j] - 1]++;
        }
        if (*max_element(cnt.begin(), cnt.end()) > 1) return false;
    }
    for (int j = 0; j < n; j++) {
        vector<int> cnt(n);
        for (int i = 0; i < n; i++) {
            if (field[i][j]) cnt[field[i][j] - 1]++;
        }
        if (*max_element(cnt.begin(), cnt.end()) > 1) return false;
    }
    for (int i0 = 0; i0 < n; i0 += k) {
        for (int j0 = 0; j0 < n; j0 += l) {
            vector<int> cnt(n);
            for (int i = i0; i < i0 + k; i++) {
                for (int j = j0; j < j0 + l; j++) {
                    if (field[i][j]) cnt[field[i][j] - 1]++;
                }
            }
            if (*max_element(cnt.begin(), cnt.end()) > 1) return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "usage: " << argv[0] << " k l cnt" << endl;
        return 1;
    }
    int k = atoi(argv[1]);
    int l = atoi(argv[2]);
    int cnt = atoi(argv[3]);
    int n = k * l;
    vector<vector<int>> field(n, vector<int>(n));
    mt19937 rnd(clock());
    for (int it = 0; it < cnt; it++) {
        while (true) {
            int id = rnd() % (n * n);
            int i = id / n, j = id % n;
            if (field[i][j]) continue;
            field[i][j] = rnd() % n + 1;
            if (!check(k, l, field)) {
                field[i][j] = 0;
                continue;
            }
            break;
        }
    }
    cout << k << ' ' << l << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << field[i][j] << ' ';
        }
        cout << endl;
    }
}
