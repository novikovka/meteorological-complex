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



    double napr(double x, double z);
    void createBullutin(std::vector<Mtd>& mtd);
    void createBullutinMts(std::vector<Mts>& mts);
    QString WindCode(int windDirection, int windSpeed);

private:
    const Coordinate* findClosest(double h,const std::vector<Coordinate>& coords);
};

#endif
