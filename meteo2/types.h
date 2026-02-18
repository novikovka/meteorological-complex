#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <vector>
#include <cmath>

constexpr double KDU = M_PI / 3000.0;
//constexpr double KDR = 180.0 / M_PI;
constexpr double KDR = 3000.0 / M_PI;
constexpr double EPS = 1e-9;

constexpr double A = 1.0;
constexpr double B = 4000.0;
constexpr double C = 100.0;
constexpr double R1 = 32.0;
constexpr double R2 = 32.0;

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
    double T{};

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
    double dh{};
    double T{};

    Mtd(double height = 0.0) : h(height) {}
};

struct Mts {
    double h{};
    double y_prev{};
    double y_next{};
    double vx{};
    double vz{};
    double wx{};
    double wz{};
    double w{};
    //double v{};
    double aw{};
    double dh{};
    double T{};

    Mts(double height = 0.0) : h(height) {}
};

#endif
