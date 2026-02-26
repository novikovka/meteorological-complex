#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <vector>
#include <cmath>
#include <unordered_map>

constexpr double KDU = M_PI / 3000.0;
//constexpr double KDR = 180.0 / M_PI;
constexpr double KDR = 3000.0 / M_PI;
constexpr double EPS = 1e-9;

//constexpr double A = 1.0;
//constexpr double B = 4000.0;
//constexpr double C = 100.0;
//constexpr double R1 = 32.0;
//constexpr double R2 = 32.0;


struct Coordinate { // посчитанные логи ветра
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

struct TemperatureRecord // логи температуры
{
    int index{};
    double QO{};
    double QT{};
    double dtp{}; // радиационная поправка

    double U0{}; // наземная влажность, %
    double T0{}; // наземная температура, цельсии
    double P0{}; // наземное давление, мб

    double dtv{}; //виртуальная поправка
    double Yt{};
    double Rt{};
    double T{};
    double T1{}; // температура + радиационная поправка
    double Tpni{};
};

struct UserConstants { // константы для расчета температуры, которые вводит пользователь
    double A;
    double B;
    double C;
    double R1;
    double R2;

    double T0;
    double U0;
    double P0;
};


/*
struct Tzone {
    double height{};
    double Hi{}; //высота середины слоя
    double Tn{};
};
*/

struct Tzone {
    double height{}; // нижняя граница зоны
    double dH{}; // тощина зоны
    double Hi{}; // высота середины слоя
    double Tn{}; // температура зоны (на средней высоте зоны)

    double TTi{}; //температура зоны - табличное значение
    double TTcpm{};
    double dTvir{}; // виртуальная поправка (высота - средняя высота зоны)
    double Tvrn{}; //температура вместе с виртуальной поправкой

    double Ttab{}; // табличное значение для средней высоты зоны

    double Pn{}; // давление в слое
    double Pi{}; //плотность П в слое
    double Pitab{}; // табличное значение плотности для высоты Hi
    double PPi{}; //действительное отклонение плотности в слое
    double PPcpm{}; //среднее отклонение плотности в слое

    Tzone() = default;

    explicit Tzone(double h)
        : height(h)
    {}
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

    //double Yt{};
    //double Rt{};
    //double T{};

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
