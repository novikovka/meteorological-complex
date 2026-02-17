#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <vector>
#include <cmath>

constexpr double KDU = M_PI / 3000.0;
//constexpr double KDR = 180.0 / M_PI;
constexpr double KDR = 3000.0 / M_PI;
constexpr double EPS = 1e-9;

struct Coordinate {
    double X{};
    double Z{};
    double H{};
    double S{};

    QString toString() const {
        return QString("X=%1 Z=%2 H=%3 S=%4")
        .arg(X, 0, 'f', 3)
            .arg(Z, 0, 'f', 3)
            .arg(H, 0, 'f', 3)
            .arg(S, 0, 'f', 3);
    }
};

struct Zone {
    double height{};
    double x{};
    double z{};
    double s{};
    double vx{};
    double vz{};
    double dh{};
    double y{};

    Zone(double h = 0.0) : height(h) {}
};

struct Mtd {
    double h{};
    double y_prev{};
    double y_next{};
    double vx{};
    double vz{};
    double v{};
    double av{};

    Mtd(double height = 0.0) : h(height) {}
};

#endif
