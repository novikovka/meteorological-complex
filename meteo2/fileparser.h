#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QString>
#include <vector>
#include "types.h"

class FileParser
{
public:
    bool parseCSV(const QString& fileName,std::vector<Coordinate>& coordinates,Zone& firstZone,Mtd& firstMtd);

private:
    void parseFirstLine(const QStringList& values,Zone& firstZone,Mtd& firstMtd);

    void parseDataLine(const QStringList& values,std::vector<Coordinate>& coordinates);
};

#endif
