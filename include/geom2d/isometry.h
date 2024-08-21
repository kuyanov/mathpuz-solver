#pragma once

#include <array>
#include <cmath>
#include "lattice.h"
#include "figure.h"

namespace Geom2d {
    struct Isometry {
        int sym;
        double rot_angle;
        point dv;
    };

    inline int cnt_isometries(const Lattice &lattice, int n) {
        return 2 * lattice.rot_ord * (4 * n - 1) * (4 * n - 1);
    }

    inline int encode_isometry(const Isometry &op, const Lattice &lattice, int n) {
        int rot = (int) round(op.rot_angle * lattice.rot_ord / (2 * M_PI));
        rot = (rot % lattice.rot_ord + lattice.rot_ord) % lattice.rot_ord;
        auto [dx, dy] = localize(lattice, op.dv);
        return ((op.sym * lattice.rot_ord + rot) * (4 * n - 1) + dx + 2 * n - 1) * (4 * n - 1) + dy + 2 * n - 1;
    }

    inline Isometry decode_isometry(int is, const Lattice &lattice, int n) {
        int dy = is % (4 * n - 1) - (2 * n - 1);
        is /= 4 * n - 1;
        int dx = is % (4 * n - 1) - (2 * n - 1);
        is /= 4 * n - 1;
        int rot = is % lattice.rot_ord;
        is /= lattice.rot_ord;
        int sym = is;
        return {.sym = sym,
                .rot_angle = 2 * M_PI * rot / lattice.rot_ord,
                .dv = lattice.vx * dx + lattice.vy * dy};
    }

    inline point apply_isometry(const Isometry &op, const point &p) {
        auto q = p;
        if (op.sym) q.y = -q.y;
        return rotate(q, op.rot_angle) + op.dv;
    }

    inline std::array<int, 3> lattice_polygon_image(const Isometry &op, const Lattice &lattice, int i, int j, int f) {
        auto polygon = lattice_polygon(lattice, i, j, f);
        for (auto &vertex: polygon) {
            vertex = apply_isometry(op, vertex);
        }
        auto inner_point = polygon[1] * (1 - 20 * eps) + (polygon[0] + polygon[2]) * 10 * eps;
        auto [i2, j2] = localize(lattice, inner_point);
        for (int f2 = 0; f2 < (int) lattice.fs.size(); f2++) {
            if (coincide(polygon, lattice_polygon(lattice, i2, j2, f2))) {
                return {i2, j2, f2};
            }
        }
        return {-1, -1, -1};
    }

    inline bool self_congruent(const Isometry &op, const Figure &figure) {
        int n = figure.n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int f = 0; f < (int) figure.lattice.fs.size(); f++) {
                    if (figure.ord[i][j][f] == -1) continue;
                    auto [i2, j2, f2] = lattice_polygon_image(op, figure.lattice, i, j, f);
                    if (i2 < 0 || i2 >= n || j2 < 0 || j2 >= n || figure.ord[i2][j2][f2] == -1) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
}
