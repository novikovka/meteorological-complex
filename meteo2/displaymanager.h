#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "types.h"
#include <QString>

class DisplayManager
{
public:
    DisplayManager();

    QString generateHeader(const CellInfo& cell);
    QString HtmlCoordinates(const CellInfo& cell, const std::vector<Coordinate>& coordinates);

    QString HtmlMtdV(const CellInfo& cell, const Zone& current_zone, const std::vector<Mtd>& mtd,
                  const std::vector<Coordinate>& coordinates);
    QString HtmlMtdAV(const CellInfo& cell, const Zone& current_zone, const std::vector<Mtd>& mtd,
                   const std::vector<Coordinate>& coordinates);
    QString HtmlMtsV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                     const std::vector<Coordinate>& coordinates);
    QString HtmlMtsAV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                      const std::vector<Coordinate>& coordinates);

    QString HtmlTTiMtd(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd,
                       const UserConstants& constants);
    QString HtmlTTiMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                       const UserConstants& constants);
    QString HtmlTTi(const CellInfo& cell,const std::vector<Zone>& zones,const UserConstants& constants);
    QString HtmlTTcpm(const CellInfo& cell,const std::vector<Zone>& zones,const UserConstants& constants);

    QString HtmlTTcpmMtd(const CellInfo& cell,const std::vector<Zone>& zones,  const std::vector<Mtd>& mtd, const UserConstants& constants);
    QString HtmlTTcpmMts(const CellInfo& cell,const std::vector<Zone>& zones, const std::vector<Mts>& mts, const UserConstants& constants);

    QString HtmlPPi(const CellInfo& cell, const std::vector<Zone>& zones);

    QString HtmlPPiMtd(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd);
    QString HtmlPPiMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts);

    QString HtmlPPcpmMtd(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd);
    QString HtmlPPcpmMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts);
};

#endif // DISPLAYMANAGER_H
