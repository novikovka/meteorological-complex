#include "fileparser.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtMath>
#include "types.h"

/*
double convertHeight(int h_code)
{
    static const std::unordered_map<int, double> heightMap =
        {
            {4,   4},
            {25,  25},
            {75,  75},

            {15,  150},
            {30,  300},
            {50,  500},
            {70,  700},
            {90,  900},

            {11, 1100},
            {14, 1400},
            {18, 1800},

            {22, 2200},
            {27, 2700},

            {35, 3500},
            {45, 4500},
            {55, 5500},

            {70, 7000},
            {90, 9000}
        };

    auto it = heightMap.find(h_code);

    if (it != heightMap.end())
        return it->second;

    return -1; // неизвестный код
}
*/

double convertHeight(int h_code)
{
    static bool first70 = true;
    static bool first90 = true;

    if (h_code == 70)
    {
        if (first70)
        {
            first70 = false;
            return 700;
        }
        else
        {
            return 7000;
        }
    }

    if (h_code == 90)
    {
        if (first90)
        {
            first90 = false;
            return 900;
        }
        else
        {
            return 9000;
        }
    }

    static const std::unordered_map<int, double> heightMap =
        {
            {4,   4},
            {25,  25},
            {75,  75},

            {15,  150},
            {30,  300},
            {50,  500},

            {11, 1100},
            {14, 1400},
            {18, 1800},

            {22, 2200},
            {27, 2700},

            {35, 3500},
            {45, 4500},
            {55, 5500}
        };

    auto it = heightMap.find(h_code);

    if (it != heightMap.end())
        return it->second;

    return -1;
}


bool FileParser::parseTxtFile(const QString& fileName,
                              std::vector<Bull_mtd>& records)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    int lineIndex = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        lineIndex++;

        // пропускаем первые две строки
        if (lineIndex <= 2)
            continue;

        if (line.isEmpty())
            continue;

        if (line.length() < 11)
            continue;

        if (line.contains("//////"))
            continue;

        Bull_mtd record;

        int h_code = line.mid(0,2).toInt();

        record.h   = convertHeight(h_code);

        record.PPi = line.mid(2,2).toDouble();
        record.TTi = line.mid(5,2).toDouble();
        record.av  = line.mid(7,2).toDouble();
        record.v   = line.mid(9,2).toDouble();

        records.push_back(record);
    }

    file.close();
    return true;
}

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

    double dglob = values[0].toDouble(); //дальность
    double aglob = values[1].toDouble(); //азимут
    double eglob = values[2].toDouble(); //угол места
    double tglob = values[3].toDouble(); //время
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
    firstMtd.av = qRound(avi / 100.0);
}

void FileParser::parseDataLine(const QStringList& values,std::vector<Coordinate>& coordinates){
    if (values.size() < 4)
        return;

    double dglob = values[0].toDouble(); //дальность
    double aglob = values[1].toDouble(); //азимут
    double eglob = values[2].toDouble(); //угол места
    double tglob = values[3].toDouble(); //время

    double cosE = cos(eglob * KDU);
    double sinE = sin(eglob * KDU);
    double cosA = cos(aglob * KDU);
    double sinA = sin(aglob * KDU);

    Coordinate coord;
    coord.X = dglob * cosE * cosA;
    coord.Z = dglob * cosE * sinA;
    double H = dglob * sinE + 0.6868e-7 * pow(dglob * cosE, 2);
    coord.H = H;
    coord.S = tglob;

    // нужно переписать эту формулу?
    coord.H_geo = ((g * m_latitude)/gc ) * ((rzem * H)/(rzem + H));

    coord.dglob = dglob;
    coord.aglob = aglob;
    coord.eglob = eglob;

    coordinates.push_back(coord);
}

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

        record.index = currentIndex;
        record.QO    = values[0].toDouble();
        record.QT    = values[1].toDouble();
        record.dtp   = values[2].toDouble();

        records.push_back(record);
    }

    file.close();
    return true;
}



