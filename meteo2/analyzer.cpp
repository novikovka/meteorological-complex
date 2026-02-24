#include "analyzer.h"
#include <cmath>
#include <QtMath>
#include <QDebug>



const Coordinate* Analyzer::findClosest(double h,const std::vector<Coordinate>& coords){
    if (coords.empty())
        return nullptr;

    const Coordinate* result = &coords[0];
    double minDiff = std::abs(coords[0].H - h);

    for (const auto& c : coords)
    {
        double diff = std::abs(c.H - h);
        if (diff < minDiff)
        {
            minDiff = diff;
            result = &c;
        }
    }

    return result;
}

void Analyzer::createZones(std::vector<Zone>& zones,const std::vector<Coordinate>& coordinates){
    auto addZones = [&](double from, double to, double step)
    {
        for (double h = from; h < to; h += step)
        {
            Zone zone(h);
            const Coordinate* closest = findClosest(h, coordinates);

            if (closest)
            {
                zone.x = closest->X;
                zone.z = closest->Z;
                zone.s = closest->S;
            }

            zones.push_back(zone);
        }
    };

    //addZones(0.0,     500.0,   100.0);
    addZones(100.0,     500.0,   100.0);
    addZones(600.0,   6000.0,  200.0);
    addZones(6000.0,  14000.0, 400.0);
    addZones(14000.0, 20000.0, 500.0);
}

void Analyzer::calculateDHmtd(std::vector<Mtd>& mtd){
    for (size_t i = 1; i < mtd.size(); ++i)
    {

        //zones[i].dh = zones[i].h - zones[i - 1].;
        mtd[i].dh = mtd[i].h - mtd[i-1].h;

    }
}

/*
void Analyzer::calculateDHmts(std::vector<Mts>& mts){
    for (size_t i = 0; i < mts.size(); ++i)
    {
        mts[i].dh = mts[i].h - mts[i-1].h;
    }
}
*/

void Analyzer::calculateDHmts(std::vector<Mts>& mts){
    for (size_t i = 1; i < mts.size(); ++i)
    {
        mts[i].dh = mts[i].h - mts[i-1].h;
    }
}


void Analyzer::calculateVk(std::vector<Zone>& zones){

    if (zones.empty())
        return;

    //zones[0].vx = 0.0;
    //zones[0].vz = 0.0;
    //zones[0].dh = 0.0;
    //zones[0].y  = 0.0;

    for (size_t i = 1; i < zones.size(); ++i)
    {
        double dt = zones[i].s - zones[i - 1].s;

        if (std::abs(dt) < EPS)
            continue;

        zones[i].vx = (zones[i].x - zones[i - 1].x) / (dt * 0.1);
        zones[i].vz = (zones[i].z - zones[i - 1].z) / (dt * 0.1);
        zones[i].dh = zones[i].height - zones[i - 1].height;
        zones[i].y  = (zones[i].height + zones[i - 1].height) / 2.0;
    }
}

void Analyzer::calculateVm(const std::vector<Zone>& zones,std::vector<Mts>& mts){
    for (size_t m = 0; m < mts.size(); ++m){
        double sumX{};
        double sumZ{};

        for (size_t k = 1; k < zones.size(); ++k){
            if((zones[k].height <= mts[m].h) && (zones[k].height > mts[m-1].h)){
                sumX += (zones[k].vx * zones[k].dh);
                sumZ += (zones[k].vz * zones[k].dh);
            }
        }
        mts[m].vx = (1/mts[m].dh) * sumX;
        mts[m].vz = (1/mts[m].dh) * sumZ;
    }
}

void Analyzer::calculateWm(std::vector<Mts>& mts)
{
    for (size_t m = 0; m < mts.size(); ++m)
    {
        double sumX{};
        double sumZ{};

        for (size_t l = 0; l <= m; ++l)
        {
            sumX += mts[l].vx * mts[l].dh;
            sumZ += mts[l].vz * mts[l].dh;
        }

        if (mts[m].h > EPS)
        {
            double wx = sumX / mts[m].h;
            double wz = sumZ / mts[m].h;

            mts[m].wx = wx;
            mts[m].wz = wz;

            mts[m].w  = std::sqrt(std::pow(wx, 2) + std::pow(wz, 2));
            mts[m].aw = qRound(napr(mts[m].wx, mts[m].wz) * KDR);

        }
    }
}


/*
void Analyzer::calculateWm(const std::vector<Zone>& zones,std::vector<Mts>& mts){

    double sumX{};
    //double sumZ{};
    for (size_t m = 1; m < mts.size(); ++m){

    }
}
*/


void Analyzer::calculateVi(const std::vector<Zone>& zones,std::vector<Mtd>& mtd){

    for (size_t i = 1; i < mtd.size(); ++i)
    {
        for (size_t k = 1; k < zones.size(); ++k)
        {
            double y_prev = zones[k - 1].y;
            double y_next = zones[k].y;

            if (mtd[i].h >= y_prev && mtd[i].h <= y_next)
            {
                double dy = y_next - y_prev;
                if (std::abs(dy) < EPS)
                    break;

                double dvx = zones[k].vx - zones[k - 1].vx;
                double dvz = zones[k].vz - zones[k - 1].vz;

                mtd[i].vx = zones[k - 1].vx +
                            (dvx / dy) * (mtd[i].h - y_prev);

                mtd[i].vz = zones[k - 1].vz +
                            (dvz / dy) * (mtd[i].h - y_prev);

                break;
            }
        }
    }
}

double Analyzer::napr(double x, double z)
{
    double a = 0;
    if (fabs(x) < 0.000001) {
        if (z > 0)
            a = 3 * M_PI_2;
        else
            a = M_PI_2;
    } else {
        if (x > 0)
            a = M_PI + atan(z / x);             // (1)
        else if (z > 0)
            a = 2 * M_PI + atan(z / x);         // (2)
        else
            a = atan(z / x);                    // (3)
    }
    return a;
}

void Analyzer::calculateV(std::vector<Mtd>& mtd){

    for (size_t i = 1; i < mtd.size(); ++i)
    {
        //mtd[i].v = std::sqrt(mtd[i].vx * mtd[i].vx + mtd[i].vz * mtd[i].vz);

        //mtd[i].av = std::atan2(mtd[i].vz, mtd[i].vx) * KDR;

        mtd[i].v = std::sqrt(pow(mtd[i].vx, 2) + pow(mtd[i].vz, 2));

        mtd[i].av = qRound(napr(mtd[i].vx, mtd[i].vz) * KDR);
    }
}

QString Analyzer::WindCode(int windDirection, int windSpeed)
{
    QString windDirectionCode =
            (windDirection == -1) ? ( "//" ):( QString("%1").arg(qRound(windDirection / 100.0), 2, 10, QChar('0')));
    QString windSpeedCode =
            (windSpeed     == -1) ? ( "//" ):( QString("%1").arg(windSpeed, 2, 10, QChar('0')));
    return windDirectionCode + " " + windSpeedCode;
}

void Analyzer::createBullutin(std::vector<Mtd>& mtd)
{
    for(auto x : mtd){
        qDebug() << WindCode(x.av, qRound(x.v));
    }
}

void Analyzer::createBullutinMts(std::vector<Mts>& mts)
{
    for(auto x : mts){
        qDebug() << WindCode(x.aw, qRound(x.w));
    }
}

/*
void Analyzer::calculateT(std::vector<TemperatureRecord>& records, UserConstants globalParam){
    for(auto r : records){
        double Yt = r.QO / r.QT;
        double Rt = (globalParam.R1 / Yt) - globalParam.R2;
        double denominator = log(pow(10,3)) + log(Rt / globalParam.A);
        double T = (globalParam.B / denominator) - globalParam.C - 273.15;
        r.Yt = Yt;
        r.Rt = Rt;
        r.T = T;
    }
}
*/

void Analyzer::calculateT(std::vector<TemperatureRecord>& records, UserConstants globalParam){
    for(auto& r : records){
        double Yt = r.QO / r.QT;
        double Rt = (globalParam.R1 / Yt) - globalParam.R2;
        double denominator = log(pow(10,3)) + log(Rt / globalParam.A);
        double T = (globalParam.B / denominator) - globalParam.C - 273.15;
        r.Yt = Yt;
        r.Rt = Rt;
        r.T = T;
    }
}

/*
void Analyzer::createTzones(std::vector<Tzone>& Tzones){
    auto addZones = [&](double from, double to, double step)
    {
        for (double h = from; h < to; h += step)
        {
            Tzone zone(h);
            Tzones.push_back(zone);
        }
    };

    addZones(0.0,     500.0,   100.0);
    addZones(100.0,     500.0,   100.0);
    addZones(600.0,   6000.0,  200.0);
    addZones(6000.0,  14000.0, 400.0);
    addZones(14000.0, 20000.0, 500.0);
}
*/

void Analyzer::createTzones(std::vector<Tzone>& Tzones)
{
    Tzones.clear();

    auto addZones = [&](double from, double to, double step)
    {
        for (double h = from; h < to; h += step)
        {
            Tzones.emplace_back(h);
        }
    };

    // первая зона
    Tzones.emplace_back(0.0);

    // до 500 м — шаг 100
    addZones(100.0, 500.0, 100.0);

    // 500–6000 м — шаг 200 (начинаем с 600)
    addZones(600.0, 6000.0, 200.0);

    // 6000–14000 м — шаг 400
    addZones(6000.0, 14000.0, 400.0);

    // 14000–20000 м — шаг 500
    addZones(14000.0, 50001.0, 500.0);
}

/*
void Analyzer::calculateTn(std::vector<Tzone>& Tzones, std::vector<TemperatureRecord>& records){

    for(auto& zone : Tzones){

        int k = 0;
        double sum = 0.0;
        int ind = 1;

        for(auto& r : records){

            if(ind == r.index){

                k += 1;
                sum += r.T;

            }else{

                zone.Tn = sum/k;
                sum = 0;
                k = 0;
                ind += 1;
            }

        }
    }
}

*/

// Метод для вычисления средних температур по индексам
void Analyzer::calculateTn(const std::vector<TemperatureRecord>& records,std::vector<Tzone>& Tzones)
{
    // Словарь: key = индекс, value = пара (сумма T, количество записей)
    std::unordered_map<int, std::pair<double, int>> sumCount;

    // 1. Просуммировать T по index
    for (const auto& rec : records) {
        sumCount[rec.index].first += rec.T;  // сумма T
        sumCount[rec.index].second += 1;    // количество записей
    }

    // 2. Записать средние значения в Tzones по порядку
    for (size_t i = 0; i < Tzones.size(); ++i) {
        int idx = static_cast<int>(i + 1); // если индексы начинаются с 1
        if (sumCount.find(idx) != sumCount.end() && sumCount[idx].second != 0) {
            Tzones[i].Tn = sumCount[idx].first / sumCount[idx].second;
        } else {
            Tzones[i].Tn = 0.0; // или другое значение по умолчанию
        }
    }
}

void Analyzer::calculateMediumHeight(std::vector<Tzone>& Tzones)
{
    for (size_t i = 1; i < Tzones.size(); ++i)
    {
        Tzones[i].Hi = (Tzones[i - 1].height + Tzones[i].height) / 2.0;
    }
}

