#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <map>

#include "types.h"

class Analyzer
{
public:
    void createZones(std::vector<Zone>& zones,const std::vector<Coordinate>& coordinates);

    void createZones(std::vector<Zone>& Zones);

    void calculateVk(std::vector<Zone>& zones);

    void calculateVi(const std::vector<Zone>& zones,std::vector<Mtd>& mtd);

    void calculateV(std::vector<Mtd>& mtd);

    void calculateVm(const std::vector<Zone>& zones,std::vector<Mts>& mts);

    void calculateWm(std::vector<Mts>& mts);

    void calculateDHmtd(std::vector<Mtd>& mtd);
    void calculateDHmts(std::vector<Mts>& mts);

    void calculateT(std::vector<TemperatureRecord>& records, UserConstants globalParam);
    void calculateTn(const std::vector<TemperatureRecord>& records,std::vector<Zone>& zones);
    void calculateMediumHeight(std::vector<Zone>& zones);
    //void calculateDTvir(std::unordered_map<double, double> Tvir, std::vector<TemperatureRecord>& records);
    void calculateTpni(std::vector<TemperatureRecord>& records);
    void calculateDeltaH(std::vector<Zone>& zones);
    void calculateTTcpm(std::vector<Zone>& zones);
    //void calculateDTvir(const std::array<double, 51>& Tvir,std::vector<TemperatureRecord>& records);
    void addRadio(std::vector<TemperatureRecord>& records);
    void calculateDTvir(std::vector<Zone>& zones, UserConstants globalParam);
    void addVir(std::vector<Zone>& zones);
    void fillTabTemperature(const std::map<double, double>& temperatureTable,std::vector<Zone>& zones);
    void calculateTTi(std::vector<Zone>& zones);
    //void interpolateTTiToMtd(const std::vector<Tzone>& Tzones,std::vector<Mtd>& mtd);
    //void interpolateTemperatureToBullutin(const std::vector<Tzone>& Tzones,std::vector<Mtd>& mtd);

    //template <typename T>
    //void interpolateTemperatureToBullutin(const std::vector<Tzone>& Tzones,std::vector<T>& bullutin);

    template<typename T>
    void interpolateTemperatureToBullutin(const std::vector<Zone>& zones,std::vector<T>& bullutin){
        if (zones.size() < 2)
            return;

        for (auto& m : bullutin)
        {
            double h = m.h;

            // Ниже минимальной высоты
            if (h <= zones.front().Hi)
            {
                m.TTi   = zones.front().TTi;
                m.TTcpm = zones.front().TTcpm;
                continue;
            }

            // Выше максимальной высоты
            if (h >= zones.back().Hi)
            {
                m.TTi   = zones.back().TTi;
                m.TTcpm = zones.back().TTcpm;
                continue;
            }

            // Поиск интервала
            for (size_t i = 0; i < zones.size() - 1; ++i)
            {
                double H1 = zones[i].Hi;
                double H2 = zones[i + 1].Hi;

                if (h >= H1 && h <= H2)
                {
                    double k = (h - H1) / (H2 - H1);

                    // Интерполяция TTi
                    m.TTi = zones[i].TTi +
                            (zones[i + 1].TTi - zones[i].TTi) * k;

                    // Интерполяция TTcpm
                    m.TTcpm = zones[i].TTcpm +
                              (zones[i + 1].TTcpm - zones[i].TTcpm) * k;

                    break;
                }
            }
        }
    }

    template<typename T>
    void interpolateDensityToBullutin(const std::vector<Zone>& zones,std::vector<T>& bullutin){
        if (zones.size() < 2)
            return;

        for (auto& m : bullutin)
        {
            double h = m.h;

            // Ниже минимальной высоты
            if (h <= zones.front().Hi)
            {
                m.PPi   = zones.front().PPi;
                m.PPcpm = zones.front().PPcpm;
                continue;
            }

            // Выше максимальной высоты
            if (h >= zones.back().Hi)
            {
                m.PPi   = zones.back().PPi;
                m.PPcpm = zones.back().PPcpm;
                continue;
            }

            // Поиск интервала
            for (size_t i = 0; i < zones.size() - 1; ++i)
            {
                double H1 = zones[i].Hi;
                double H2 = zones[i + 1].Hi;

                if (h >= H1 && h <= H2)
                {
                    double k = (h - H1) / (H2 - H1);

                    // Интерполяция TTi
                    m.PPi = zones[i].PPi +
                            (zones[i + 1].PPi - zones[i].PPi) * k;

                    // Интерполяция TTcpm
                    m.PPcpm = zones[i].PPcpm +
                              (zones[i + 1].PPcpm - zones[i].PPcpm) * k;

                    break;
                }
            }
        }
    }

    //давление и плотность
    void fillTabDensity(const std::map<double, double>& DensityTable,std::vector<Zone>& zones);
    void calculatePn(std::vector<Zone>& zones, UserConstants globalParam);
    void calculatePi(std::vector<Zone>& zones);
    void calculatePPi(std::vector<Zone>& zones);
    void calculatePPcpm(std::vector<Zone>& zones);

    //вертикальная устойчивость

    void calculateTforR(std::vector<Zone>& zones);

    double napr(double x, double z);
    void createBullutin(std::vector<Mtd>& mtd);
    void createBullutinMts(std::vector<Mts>& mts);
    QString WindCode(int windDirection, int windSpeed);

private:
    const Coordinate* findClosest(double h,const std::vector<Coordinate>& coords);
};

#endif
