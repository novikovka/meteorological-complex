#include "fileparser.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtMath>

bool FileParser::parseCSV(const QString& fileName,std::vector<Coordinate>& coordinates,Zone& firstZone,Mtd& firstMtd){
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    bool isFirstLine = true;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList values = line.split(',');

        if (isFirstLine)
        {
            parseFirstLine(values, firstZone, firstMtd);
            isFirstLine = false;
        }
        else
        {
            parseDataLine(values, coordinates);
        }
    }

    file.close();
    return true;
}

void FileParser::parseFirstLine(const QStringList& values,Zone& firstZone,Mtd& firstMtd){

    if (values.size() < 6)
        return;

    double dglob = values[0].toDouble();
    double aglob = values[1].toDouble();
    double eglob = values[2].toDouble();
    double tglob = values[3].toDouble();
    double avi   = values[4].toDouble();
    double vi    = values[5].toDouble();

    firstZone.x = dglob * cos(eglob * KDU) * cos(aglob * KDU);
    firstZone.z = dglob * cos(eglob * KDU) * sin(aglob * KDU);
    firstZone.s = tglob;

    double direction = avi * 6.0;
    if (direction >= 360.0)
        direction -= 360.0;

    direction = qDegreesToRadians(direction);

    firstZone.vx = vi * cos(direction);
    firstZone.vz = vi * sin(direction);

    firstMtd.vx = vi * cos(direction);
    firstMtd.vz = vi * sin(direction);
    firstMtd.v  = vi;
    firstMtd.av = avi;
}

void FileParser::parseDataLine(const QStringList& values,std::vector<Coordinate>& coordinates){
    if (values.size() < 4)
        return;

    double dglob = values[0].toDouble();
    double aglob = values[1].toDouble();
    double eglob = values[2].toDouble();
    double tglob = values[3].toDouble();

    double cosE = cos(eglob * KDU);
    double sinE = sin(eglob * KDU);
    double cosA = cos(aglob * KDU);
    double sinA = sin(aglob * KDU);

    Coordinate coord;
    coord.X = dglob * cosE * cosA;
    coord.Z = dglob * cosE * sinA;
    coord.H = dglob * sinE + 0.6868e-7 * pow(dglob * cosE, 2);
    coord.S = tglob;

    coordinates.push_back(coord);
}

/*
bool FileParser::parseTemperatureCSV(const QString& fileName,std::vector<TemperatureRecord>& records)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList values = line.split(',');

        if (values.size() < 3)
            continue;

        TemperatureRecord record;
        record.QO  = values[0].toDouble();
        record.QT  = values[1].toDouble();
        record.dtp = values[2].toDouble();

        //double Yt = QO / QT;
        //double Rt = (globalParam.R1 / Yt) - globalParam.R2;
        //double

        records.push_back(record);
    }

    file.close();
    return true;
}
*/

bool FileParser::parseTemperatureCSV(const QString& fileName,
                                     std::vector<TemperatureRecord>& records)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    int currentIndex = 1;   // начинаем с 1
    bool previousWasEmpty = false;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        // если строка пустая
        if (line.isEmpty())
        {
            // увеличиваем индекс только один раз на блок
            if (!previousWasEmpty)
            {
                currentIndex++;
                previousWasEmpty = true;
            }
            continue;
        }

        previousWasEmpty = false;

        QStringList values = line.split(',');

        if (values.size() < 3)
            continue;

        TemperatureRecord record;

        record.index = currentIndex;   // ← вот главное
        record.QO    = values[0].toDouble();
        record.QT    = values[1].toDouble();
        record.dtp   = values[2].toDouble();

        records.push_back(record);
    }

    file.close();
    return true;
}



