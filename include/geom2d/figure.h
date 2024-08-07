#pragma once

#include <istream>
#include <vector>
#include "lattice.h"

namespace Geom2d {
    struct Figure {
        Lattice lattice;
        int n, s;
        std::vector<std::vector<std::vector<int>>> ord;
    };

    inline Figure read_figure(std::istream &in, const Lattice &lattice) {
        int n;
        in >> n;
        int cnt_f = (int) lattice.fs.size();
        std::vector<std::vector<std::vector<int>>> ord(
                n, std::vector<std::vector<int>>(n, std::vector<int>(cnt_f, -1)));
        int s = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int f = 0; f < cnt_f; f++) {
                    int x;
                    in >> x;
                    if (x) ord[i][j][f] = s++;
                }
            }
        }
        return {.lattice = lattice,
                .n = n,
                .s = s,
                .ord = ord};
    }
}
