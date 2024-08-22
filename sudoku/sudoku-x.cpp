#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

void put_set(int r, const vector<vector<int>> &sets, vector<set<int>> &cov, vector<bool> &sol,
             set<int> &active_sets, set<int> &active_vs, vector<int> &removed_sets) {
    sol[r] = true;
    for (int j: sets[r]) {
        while (!cov[j].empty()) {
            int i = *cov[j].begin();
            for (int elem: sets[i]) {
                cov[elem].erase(i);
            }
            active_sets.erase(i);
            removed_sets.push_back(i);
        }
        active_vs.erase(j);
    }
}

void rollback(const vector<vector<int>> &sets, vector<set<int>> &cov, vector<bool> &sol,
              set<int> &active_sets, set<int> &active_vs, vector<int> &removed_sets) {
    for (int i: removed_sets) {
        active_sets.insert(i);
        sol[i] = false;
        for (int j: sets[i]) {
            cov[j].insert(i);
            active_vs.insert(j);
        }
    }
    removed_sets.clear();
}

int cnt_intersecting(int r, const vector<vector<int>> &sets, vector<set<int>> &cov) {
    set<int> inter;
    for (int c: sets[r]) {
        for (int i: cov[c]) {
            inter.insert(i);
        }
    }
    return (int) inter.size();
}

bool algorithm_x(const vector<vector<int>> &sets, vector<set<int>> &cov, vector<bool> &sol,
                 set<int> &active_sets, set<int> &active_vs) {
    vector<pair<int, int>> ord;
    for (int j: active_vs) {
        ord.emplace_back((int) cov[j].size(), j);
    }
    sort(ord.begin(), ord.end());
    vector<int> pre_removed_sets;
    while (!ord.empty() && ord[0].first == 1) {
        set<int> must_take;
        for (const auto &[cnt, j]: ord) {
            if (cnt == 1) {
                must_take.insert(*cov[j].begin());
            }
        }
        for (int s: must_take) {
            if (!active_sets.contains(s)) {
                rollback(sets, cov, sol, active_sets, active_vs, pre_removed_sets);
                return false;
            }
            put_set(s, sets, cov, sol, active_sets, active_vs, pre_removed_sets);
        }
        ord.clear();
        for (int j: active_vs) {
            ord.emplace_back((int) cov[j].size(), j);
        }
        sort(ord.begin(), ord.end());
    }
    if (ord.empty()) {
        return true;
    }
    if (ord[0].first == 0) {
        rollback(sets, cov, sol, active_sets, active_vs, pre_removed_sets);
        return false;
    }
    int c = ord[0].second;
    auto rs = cov[c];
    vector<pair<int, int>> priors;
    for (int r: rs) {
        priors.emplace_back(cnt_intersecting(r, sets, cov), r);
    }
    sort(priors.rbegin(), priors.rend());
    for (auto [_, r]: priors) {
        vector<int> removed_sets;
        put_set(r, sets, cov, sol, active_sets, active_vs, removed_sets);
        if (algorithm_x(sets, cov, sol, active_sets, active_vs)) {
            return true;
        }
        rollback(sets, cov, sol, active_sets, active_vs, removed_sets);
    }
    rollback(sets, cov, sol, active_sets, active_vs, pre_removed_sets);
    return false;
}

bool exact_cover(int n, const vector<vector<int>> &sets, vector<bool> &sol) {
    int m = (int) sets.size();
    vector<set<int>> cov(n);
    for (int i = 0; i < m; i++) {
        for (int j: sets[i]) {
            cov[j].insert(i);
        }
    }
    set<int> active_sets, active_vs;
    for (int i = 0; i < m; i++) active_sets.insert(i);
    for (int j = 0; j < n; j++) active_vs.insert(j);
    vector<int> removed_sets;
    for (int i = 0; i < m; i++) {
        if (sol[i]) {
            put_set(i, sets, cov, sol, active_sets, active_vs, removed_sets);
        }
    }
    return algorithm_x(sets, cov, sol, active_sets, active_vs);
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
    bool found = exact_cover(cnt_vs, sets, sol);
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
