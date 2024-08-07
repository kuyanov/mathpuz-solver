#pragma once

#include <cmath>
#include <compare>
#include <istream>
#include <ostream>
#include <vector>

namespace Geom2d {
    const double eps = 1e-4;

    struct point {
        double x, y;
    };

    std::istream &operator>>(std::istream &in, point &p) {
        return in >> p.x >> p.y;
    }

    std::ostream &operator<<(std::ostream &out, point &p) {
        return out << p.x << ' ' << p.y;
    }

    bool operator==(const point &a, const point &b) {
        return std::abs(a.x - b.x) < eps && std::abs(a.y - b.y) < eps;
    }

    std::strong_ordering operator<=>(const point &a, const point &b) {
        if (a.x < b.x - eps) return std::strong_ordering::less;
        if (a.x > b.x + eps) return std::strong_ordering::greater;
        if (a.y < b.y - eps) return std::strong_ordering::less;
        if (a.y > b.y + eps) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    point operator+(const point &a, const point &b) {
        return {a.x + b.x, a.y + b.y};
    }

    point operator-(const point &a, const point &b) {
        return {a.x - b.x, a.y - b.y};
    }

    point operator*(const point &a, double b) {
        return {a.x * b, a.y * b};
    }

    point operator/(const point &a, double b) {
        return {a.x / b, a.y / b};
    }

    double operator*(const point &a, const point &b) {
        return a.x * b.x + a.y * b.y;
    }

    double operator^(const point &a, const point &b) {
        return a.x * b.y - a.y * b.x;
    }

    double angle(const point &a, const point &b) {
        return atan2(a ^ b, a * b);
    }

    point rotate(const point &a, double angle) {
        return {cos(angle) * a.x - sin(angle) * a.y,
                sin(angle) * a.x + cos(angle) * a.y};
    }

    bool coincide(const std::vector<point> &polygon1, const std::vector<point> &polygon2) {
        if (polygon1.size() != polygon2.size()) return false;
        size_t n = polygon1.size();
        size_t delta = std::find(polygon1.begin(), polygon1.end(), polygon2[0]) - polygon1.begin();
        if (delta == n) return false;
        bool ok1 = true, ok2 = true;
        for (size_t i = 0; i < n; i++) {
            if (polygon2[i] != polygon1[(delta + i) % n]) {
                ok1 = false;
            }
            if (polygon2[i] != polygon1[(n + delta - i) % n]) {
                ok2 = false;
            }
        }
        return ok1 || ok2;
    }
}
