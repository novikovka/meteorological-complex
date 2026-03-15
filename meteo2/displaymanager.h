#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "types.h"
#include <QString>

class DisplayManager
{
public:
    DisplayManager();

    //QString generateHtml(const CellInfo& cell);
    QString generateHtml(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd);
    QString generateHtmlMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts);
    QString HtmlV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd);
    QString HtmlAV(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd);
    QString HtmlTTi(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd);
};

#endif // DISPLAYMANAGER_H
