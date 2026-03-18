#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "types.h"
#include <QString>

class DisplayManager
{
public:
    DisplayManager();

    // Заголовок для любого параметра любого бюллетеня
    QString generateHeader(const CellInfo& cell);
    QString HtmlCoordinates(const CellInfo& cell, const std::vector<Coordinate>& coordinates);

    QString HtmlMtdV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd,
                  const std::vector<Coordinate>& coordinates);
    QString HtmlMtdAV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd,
                   const std::vector<Coordinate>& coordinates);
    QString HtmlMtsV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                     const std::vector<Coordinate>& coordinates);
    QString HtmlMtsAV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                      const std::vector<Coordinate>& coordinates);


    QString HtmlTTiMtd(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd,
                       const UserConstants& constants, std::vector<TemperatureRecord>& records);
    QString HtmlTTiMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                       const UserConstants& constants, std::vector<TemperatureRecord>& records);
    QString HtmlTTi(const CellInfo& cell,const std::vector<Zone>& zones,
                                    const UserConstants& constants, std::vector<TemperatureRecord>& records);
    QString HtmlPPi(const CellInfo& cell, const std::vector<Zone>& zones);
};

#endif // DISPLAYMANAGER_H
