#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QString>
#include <vector>
#include "types.h"

class FileParser
{
public:

    bool parseTxtFile(const QString& fileName,
                      std::vector<Bull_mtd>& records);

    bool parseCSV(const QString& fileName,std::vector<Coordinate>& coordinates,Zone& firstZone,Mtd& firstMtd);

    bool parseTemperatureCSV(const QString& fileName,std::vector<TemperatureRecord>& records);


private:
    void parseFirstLine(const QStringList& values,Zone& firstZone,Mtd& firstMtd);

    void parseDataLine(const QStringList& values,std::vector<Coordinate>& coordinates);
};

#endif
