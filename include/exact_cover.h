#pragma once

#include <algorithm>
#include <deque>
#include <list>
#include <random>
#include <set>
#include <thread>
#include <vector>

#ifdef APPLE
#include "EvilThread.h"
#endif

using It = std::list<int>::iterator;

struct AlgXState {
    const std::vector<std::vector<int>> &sets;
    std::vector<std::list<int>> cov;
    std::vector<std::vector<It>> cov_loc;
    std::list<int> active_sets, active_vs;
    std::vector<It> active_sets_loc, active_vs_loc;
    std::vector<bool> sol;

    void put_set(int s, std::vector<int> &removed_sets) {
        sol[s] = true;
        for (int j: sets[s]) {
            while (!cov[j].empty()) {
                int i = *cov[j].begin();
                for (size_t ind = 0; ind < sets[i].size(); ind++) {
                    cov[sets[i][ind]].erase(cov_loc[i][ind]);
                }
                active_sets.erase(active_sets_loc[i]);
                active_sets_loc[i] = active_sets.end();
                removed_sets.push_back(i);
            }
            active_vs.erase(active_vs_loc[j]);
        }
    }

    void rollback(std::vector<int> &removed_sets) {
        for (int i: removed_sets) {
            active_sets_loc[i] = active_sets.insert(active_sets.end(), i);
            if (sol[i]) {
                sol[i] = false;
                for (int j: sets[i]) {
                    active_vs_loc[j] = active_vs.insert(active_vs.end(), j);
                }
            }
            for (size_t ind = 0; ind < sets[i].size(); ind++) {
                int j = sets[i][ind];
                cov_loc[i][ind] = cov[j].insert(cov[j].end(), i);
            }
        }
        removed_sets.clear();
    }

    int prior(int s) {
        std::set<int> inter;
        for (int j: sets[s]) {
            for (int i: cov[j]) {
                inter.insert(i);
            }
        }
        return (int) inter.size();
    }

    explicit AlgXState(int n, const std::vector<std::vector<int>> &sets, const std::vector<bool> &sol)
            : sets(sets), sol(sol) {
        int m = (int) sets.size();
        cov.resize(n);
        cov_loc.resize(m);
        for (int i = 0; i < m; i++) {
            for (int j: sets[i]) {
                cov_loc[i].push_back(cov[j].insert(cov[j].end(), i));
            }
        }
        active_sets_loc.resize(m);
        active_vs_loc.resize(n);
        for (int i = 0; i < m; i++) {
            active_sets_loc[i] = active_sets.insert(active_sets.end(), i);
        }
        for (int j = 0; j < n; j++) {
            active_vs_loc[j] = active_vs.insert(active_vs.end(), j);
        }
        std::vector<int> removed_sets;
        for (int i = 0; i < m; i++) {
            if (sol[i]) {
                put_set(i, removed_sets);
            }
        }
    }

    AlgXState(const AlgXState &other)
            : sets(other.sets), active_sets(other.active_sets), active_vs(other.active_vs), sol(other.sol) {
        int n = (int) other.cov.size(), m = (int) sets.size();
        cov.resize(n);
        cov_loc.resize(m);
        for (int i: active_sets) {
            for (size_t ind = 0; ind < sets[i].size(); ind++) {
                int j = sets[i][ind];
                cov_loc[i].push_back(cov[j].insert(cov[j].end(), i));
            }
        }
        active_sets_loc.resize(m, active_sets.end());
        active_vs_loc.resize(n);
        for (auto it = active_sets.begin(); it != active_sets.end(); ++it) {
            active_sets_loc[*it] = it;
        }
        for (auto it = active_vs.begin(); it != active_vs.end(); ++it) {
            active_vs_loc[*it] = it;
        }
    }

    AlgXState(AlgXState &&other) = default;
};

bool algorithm_x(AlgXState &state, const std::atomic<bool> &ready, bool shuffle) {
    if (ready) return false;
    std::vector<std::pair<size_t, int>> ord;
    ord.reserve(state.active_vs.size());
    for (int j: state.active_vs) {
        ord.emplace_back(state.cov[j].size(), j);
    }
    std::sort(ord.begin(), ord.end());
    std::vector<int> pre_removed_sets;
    while (!ord.empty() && ord[0].first == 1) {
        std::set<int> must_take;
        for (const auto &[cnt, j]: ord) {
            if (cnt == 1) {
                must_take.insert(*state.cov[j].begin());
            }
        }
        for (int s: must_take) {
            if (state.active_sets_loc[s] == state.active_sets.end()) {
                state.rollback(pre_removed_sets);
                return false;
            }
            state.put_set(s, pre_removed_sets);
        }
        ord.clear();
        for (int j: state.active_vs) {
            ord.emplace_back(state.cov[j].size(), j);
        }
        std::sort(ord.begin(), ord.end());
    }
    if (ord.empty()) {
        return true;
    }
    if (ord[0].first == 0) {
        state.rollback(pre_removed_sets);
        return false;
    }
    int c = ord[0].second;
    auto rs = state.cov[c];
    std::vector<std::pair<int, int>> priors;
    for (int r: rs) {
        priors.emplace_back(state.prior(r), r);
    }
    std::sort(priors.begin(), priors.end());
    if (shuffle) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        std::shuffle(priors.begin(), priors.end(), rnd);
    }
    for (auto [_, r]: priors) {
        std::vector<int> removed_sets;
        state.put_set(r, removed_sets);
        if (algorithm_x(state, ready, shuffle)) {
            return true;
        }
        state.rollback(removed_sets);
    }
    state.rollback(pre_removed_sets);
    return false;
}

bool exact_cover(int n, const std::vector<std::vector<int>> &sets, std::vector<bool> &sol,
                 int num_threads = 1) {
    std::deque<AlgXState> q;
    q.emplace_back(n, sets, sol);
    while (!q.empty() && q.size() < num_threads) {
        auto cur_state = std::move(q.front());
        q.pop_front();
        int c = -1;
        for (int j: cur_state.active_vs) {
            if (c == -1 || cur_state.cov[j].size() < cur_state.cov[c].size()) {
                c = j;
            }
        }
        if (c == -1) {
            sol = cur_state.sol;
            return true;
        }
        for (int r: cur_state.cov[c]) {
            auto new_state = cur_state;
            std::vector<int> removed_sets;
            new_state.put_set(r, removed_sets);
            q.emplace_back(std::move(new_state));
        }
    }
    std::vector<std::thread> threads(num_threads);
    std::atomic<int> state_id = -1;
    std::atomic<bool> ready = false;
    for (int tid = 0; tid < num_threads; tid++) {
#ifdef APPLE
        threads[tid] = evil::create_thread(1 << 24, [&](int tid) {
#else
            threads[tid] = std::thread([&](int tid) {
#endif
            for (int sid = tid; sid < (int) q.size(); sid += num_threads) {
                bool found = algorithm_x(q[sid], ready, false);
                if (found) {
                    ready = true;
                    state_id = sid;
                    break;
                }
            }
        }, tid);
    }
    for (int tid = 0; tid < num_threads; tid++) {
        threads[tid].join();
    }
    if (state_id == -1) {
        return false;
    }
    sol = q[state_id].sol;
    return true;
}
