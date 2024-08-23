#pragma once

#include <algorithm>
#include <list>
#include <set>
#include <vector>

using It = std::list<int>::iterator;

void put_set(int s, const std::vector<std::vector<int>> &sets, std::vector<std::list<int>> &cov,
             std::vector<std::vector<It>> &loc, std::vector<bool> &sol,
             std::set<int> &active_sets, std::set<int> &active_vs, std::vector<int> &removed_sets) {
    sol[s] = true;
    for (int j: sets[s]) {
        while (!cov[j].empty()) {
            int i = *cov[j].begin();
            for (int ind = 0; ind < (int) sets[i].size(); ind++) {
                cov[sets[i][ind]].erase(loc[i][ind]);
            }
            active_sets.erase(i);
            removed_sets.push_back(i);
        }
        active_vs.erase(j);
    }
}

void rollback(const std::vector<std::vector<int>> &sets, std::vector<std::list<int>> &cov,
              std::vector<std::vector<It>> &loc, std::vector<bool> &sol,
              std::set<int> &active_sets, std::set<int> &active_vs, std::vector<int> &removed_sets) {
    for (int i: removed_sets) {
        active_sets.insert(i);
        sol[i] = false;
        for (int ind = 0; ind < (int) sets[i].size(); ind++) {
            int j = sets[i][ind];
            loc[i][ind] = cov[j].insert(cov[j].end(), i);
            active_vs.insert(j);
        }
    }
    removed_sets.clear();
}

int cnt_intersecting(int r, const std::vector<std::vector<int>> &sets, std::vector<std::list<int>> &cov) {
    std::set<int> inter;
    for (int c: sets[r]) {
        for (int i: cov[c]) {
            inter.insert(i);
        }
    }
    return (int) inter.size();
}

bool algorithm_x(const std::vector<std::vector<int>> &sets, std::vector<std::list<int>> &cov,
                 std::vector<std::vector<It>> &loc, std::vector<bool> &sol,
                 std::set<int> &active_sets, std::set<int> &active_vs) {
    std::vector<std::pair<int, int>> ord;
    ord.reserve(active_vs.size());
    for (int j: active_vs) {
        ord.emplace_back((int) cov[j].size(), j);
    }
    std::sort(ord.begin(), ord.end());
    std::vector<int> pre_removed_sets;
    while (!ord.empty() && ord[0].first == 1) {
        std::set<int> must_take;
        for (const auto &[cnt, j]: ord) {
            if (cnt == 1) {
                must_take.insert(*cov[j].begin());
            }
        }
        for (int s: must_take) {
            if (!active_sets.contains(s)) {
                rollback(sets, cov, loc, sol, active_sets, active_vs, pre_removed_sets);
                return false;
            }
            put_set(s, sets, cov, loc, sol, active_sets, active_vs, pre_removed_sets);
        }
        ord.clear();
        for (int j: active_vs) {
            ord.emplace_back((int) cov[j].size(), j);
        }
        std::sort(ord.begin(), ord.end());
    }
    if (ord.empty()) {
        return true;
    }
    if (ord[0].first == 0) {
        rollback(sets, cov, loc, sol, active_sets, active_vs, pre_removed_sets);
        return false;
    }
    int c = ord[0].second;
    auto rs = cov[c];
    std::vector<std::pair<int, int>> priors;
    for (int r: rs) {
        priors.emplace_back(cnt_intersecting(r, sets, cov), r);
    }
    std::sort(priors.rbegin(), priors.rend());
    for (auto [_, r]: priors) {
        std::vector<int> removed_sets;
        put_set(r, sets, cov, loc, sol, active_sets, active_vs, removed_sets);
        if (algorithm_x(sets, cov, loc, sol, active_sets, active_vs)) {
            return true;
        }
        rollback(sets, cov, loc, sol, active_sets, active_vs, removed_sets);
    }
    rollback(sets, cov, loc, sol, active_sets, active_vs, pre_removed_sets);
    return false;
}

bool exact_cover(int n, const std::vector<std::vector<int>> &sets, std::vector<bool> &sol) {
    int m = (int) sets.size();
    std::vector<std::list<int>> cov(n);
    std::vector<std::vector<It>> loc(m);
    for (int i = 0; i < m; i++) {
        loc[i].resize(sets[i].size());
        for (int ind = 0; ind < (int) sets[i].size(); ind++) {
            int j = sets[i][ind];
            loc[i][ind] = cov[j].insert(cov[j].end(), i);
        }
    }
    std::set<int> active_sets, active_vs;
    for (int i = 0; i < m; i++) active_sets.insert(i);
    for (int j = 0; j < n; j++) active_vs.insert(j);
    std::vector<int> removed_sets;
    for (int i = 0; i < m; i++) {
        if (sol[i]) {
            put_set(i, sets, cov, loc, sol, active_sets, active_vs, removed_sets);
        }
    }
    return algorithm_x(sets, cov, loc, sol, active_sets, active_vs);
}
