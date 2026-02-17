#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include "types.h"

class Analyzer
{
public:
    void createZones(std::vector<Zone>& zones,const std::vector<Coordinate>& coordinates);

    void calculateVk(std::vector<Zone>& zones);

    void calculateVi(const std::vector<Zone>& zones,std::vector<Mtd>& mtd);

    void calculateV(std::vector<Mtd>& mtd);

    double napr(double x, double z);
    void createBullutin(std::vector<Mtd>& mtd);
    QString WindCode(int windDirection, int windSpeed);

private:
    const Coordinate* findClosest(double h,const std::vector<Coordinate>& coords);
};

#endif
