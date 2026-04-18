#include "fileparser.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtMath>
#include "types.h"
#include <QRegularExpression>
#include <QDebug>

// Глобальные статические переменные для отслеживания использования кодов высот
static bool used_12 = false;
static bool used_14 = false;
static bool used_18 = false;
static bool used_22 = false;
static bool used_26 = false;
static bool used_30 = false;

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

double convertHeightMts(int h_code, bool isFourDigitFormat)
{
    // Для 4-значного формата (HHPP) - это всегда высоты ДО 10000м
    if (isFourDigitFormat)
    {
        static const std::unordered_map<int, double> heightMapFourDigit =
            {
                {2,   200},
                {4,   400},
                {8,   800},
                {12, 1200},
                {14, 1400},
                {16, 1600},
                {18, 1800},
                {20, 2000},
                {22, 2200},
                {24, 2400},
                {26, 2600},
                {30, 3000},
                {40, 4000},
                {50, 5000},
                {60, 6000},
                {80, 8000},
                {10, 10000}
            };

        auto it = heightMapFourDigit.find(h_code);
        if (it != heightMapFourDigit.end())
            return it->second;
    }
    else // Для 2-значного формата (HH) - это высоты ОТ 10000м и выше
    {
        static const std::unordered_map<int, double> heightMapTwoDigit =
            {
                {12, 12000},
                {14, 14000},
                {18, 18000},
                {22, 22000},
                {26, 26000},
                {30, 30000}
            };

        auto it = heightMapTwoDigit.find(h_code);
        if (it != heightMapTwoDigit.end())
            return it->second;
    }

    qDebug() << "Unknown height code:" << h_code << "format:" << (isFourDigitFormat ? "4-digit" : "2-digit");
    return -1;
}

// Функция для сброса флагов (добавьте её объявление в fileparser.h)
void resetHeightMtsFlags()
{
    used_12 = false;
    used_14 = false;
    used_18 = false;
    used_22 = false;
    used_26 = false;
    used_30 = false;
}

bool FileParser::parseMeteoAverage(const QString& fileName,
                                   std::vector<Bull_mts>& records)
{
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

        // Пропускаем первую строку с "МЕТЕО ..."
        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        // Встретили "1818" - конец бюллетеня
        if (line == "1818")
            break;

        Bull_mts record;

        // Формат строки: "0201-555408-" или "12-193309-"
        int dashPos = line.indexOf('-');
        if (dashPos == -1)
            continue;

        QString codePart = line.left(dashPos);      // часть до первого дефиса
        QString dataPart = line.mid(dashPos + 1);   // часть после первого дефиса

        // Убираем последний дефис в конце строки, если есть
        if (dataPart.endsWith('-'))
            dataPart.chop(1);

        // Парсим данные: TT DD VV (всегда 6 символов)
        if (dataPart.length() >= 6)
        {
            record.TTcpm = dataPart.mid(0, 2).toDouble();   // температура
            record.aw    = dataPart.mid(2, 2).toDouble();   // направление ветра
            record.w     = dataPart.mid(4, 2).toDouble();   // скорость ветра
        }
        else
        {
            continue;
        }

        // Определяем формат по длине codePart
        if (codePart.length() == 4)
        {
            // Формат HHPP - высота (2 цифры) + давление (2 цифры)
            int h_code = codePart.mid(0, 2).toInt();
            record.h = convertHeightMts(h_code, true);  // true = 4-значный формат
            record.PPcpm = codePart.mid(2, 2).toDouble();

            qDebug() << "4-char format:" << codePart
                     << "h_code=" << h_code
                     << "height=" << record.h
                     << "pressure=" << record.PPcpm;
        }
        else if (codePart.length() == 2)
        {
            // Формат HH - только высота (высоты от 10000м и выше)
            int h_code = codePart.toInt();
            record.h = convertHeightMts(h_code, false);  // false = 2-значный формат
            record.PPcpm = NAN;

            qDebug() << "2-char format:" << codePart
                     << "h_code=" << h_code
                     << "height=" << record.h;
        }
        else
        {
            qDebug() << "Unknown format:" << codePart << "length=" << codePart.length();
            continue;
        }

        records.push_back(record);
    }

    file.close();
    qDebug() << "Total records parsed:" << records.size();
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
