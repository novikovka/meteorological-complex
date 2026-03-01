#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <map>

#include "types.h"

class Analyzer
{
public:
    void createZones(std::vector<Zone>& zones,const std::vector<Coordinate>& coordinates);

    void createTzones(std::vector<Tzone>& Tzones);

    void calculateVk(std::vector<Zone>& zones);

    void calculateVi(const std::vector<Zone>& zones,std::vector<Mtd>& mtd);

    void calculateV(std::vector<Mtd>& mtd);

    void calculateVm(const std::vector<Zone>& zones,std::vector<Mts>& mts);

    void calculateWm(std::vector<Mts>& mts);

    void calculateDHmtd(std::vector<Mtd>& mtd);
    void calculateDHmts(std::vector<Mts>& mts);

    void calculateT(std::vector<TemperatureRecord>& records, UserConstants globalParam);
    void calculateTn(const std::vector<TemperatureRecord>& records,std::vector<Tzone>& Tzones);
    void calculateMediumHeight(std::vector<Tzone>& Tzones);
    //void calculateDTvir(std::unordered_map<double, double> Tvir, std::vector<TemperatureRecord>& records);
    void calculateTpni(std::vector<TemperatureRecord>& records);
    void calculateDeltaH(std::vector<Tzone>& Tzones);
    void calculateTTcpm(std::vector<Tzone>& Tzones);
    //void calculateDTvir(const std::array<double, 51>& Tvir,std::vector<TemperatureRecord>& records);
    void addRadio(std::vector<TemperatureRecord>& records);
    void calculateDTvir(std::vector<Tzone>& Tzones, UserConstants globalParam);
    void addVir(std::vector<Tzone>& Tzones);
    void fillTabTemperature(const std::map<double, double>& temperatureTable,std::vector<Tzone>& zones);
    void calculateTTi(std::vector<Tzone>& Tzones);
    //void interpolateTTiToMtd(const std::vector<Tzone>& Tzones,std::vector<Mtd>& mtd);
    //void interpolateTemperatureToBullutin(const std::vector<Tzone>& Tzones,std::vector<Mtd>& mtd);

    //template <typename T>
    //void interpolateTemperatureToBullutin(const std::vector<Tzone>& Tzones,std::vector<T>& bullutin);

    template<typename T>
    void interpolateTemperatureToBullutin(const std::vector<Tzone>& Tzones,std::vector<T>& bullutin){
        if (Tzones.size() < 2)
            return;

        for (auto& m : bullutin)
        {
            double h = m.h;

            // Ниже минимальной высоты
            if (h <= Tzones.front().Hi)
            {
                m.TTi   = Tzones.front().TTi;
                m.TTcpm = Tzones.front().TTcpm;
                continue;
            }

            // Выше максимальной высоты
            if (h >= Tzones.back().Hi)
            {
                m.TTi   = Tzones.back().TTi;
                m.TTcpm = Tzones.back().TTcpm;
                continue;
            }

            // Поиск интервала
            for (size_t i = 0; i < Tzones.size() - 1; ++i)
            {
                double H1 = Tzones[i].Hi;
                double H2 = Tzones[i + 1].Hi;

                if (h >= H1 && h <= H2)
                {
                    double k = (h - H1) / (H2 - H1);

                    // Интерполяция TTi
                    m.TTi = Tzones[i].TTi +
                            (Tzones[i + 1].TTi - Tzones[i].TTi) * k;

                    // Интерполяция TTcpm
                    m.TTcpm = Tzones[i].TTcpm +
                              (Tzones[i + 1].TTcpm - Tzones[i].TTcpm) * k;

                    break;
                }
            }
        }
    }

    //давление и плотность
    void fillTabDensity(const std::map<double, double>& DensityTable,std::vector<Tzone>& zones);
    void calculatePn(std::vector<Tzone>& zones, UserConstants globalParam);
    void calculatePi(std::vector<Tzone>& zones);
    void calculatePPi(std::vector<Tzone>& zones);
    void calculatePPcpm(std::vector<Tzone>& zones);


    double napr(double x, double z);
    void createBullutin(std::vector<Mtd>& mtd);
    void createBullutinMts(std::vector<Mts>& mts);
    QString WindCode(int windDirection, int windSpeed);

private:
    const Coordinate* findClosest(double h,const std::vector<Coordinate>& coords);
};

#endif
