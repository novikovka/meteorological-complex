#include "displaymanager.h"
#include <cmath>

DisplayManager::DisplayManager()
{
}

QString DisplayManager::generateHeader(const CellInfo& cell){

    QString html;

    QString type_bull;

    if (cell.tableName == "TableMtd")
        type_bull = "Метеодействительный";
    else if (cell.tableName == "TableMts")
        type_bull = "Метеосредний";
    else
        type_bull = "Неизвестно";

    QString paramName = cell.columnName;

    html += "<h2>Информация о параметре</h2>";

    html += QString("<p><b>Бюллетень:</b> %1</p>").arg(type_bull);
    html += QString("<p><b>Высота:</b> %1 метров</p>").arg(cell.height);
    html += QString("<p><b>Параметр:</b> %1</p>").arg(paramName);
    html += QString("<p><b>Значение:</b> %1 м/с</p>").arg(cell.cellValue);

    html += "<hr>";
    html += "<h3>Последовательность расчетов</h3>";

    return html;
}

QString DisplayManager::HtmlCoordinates(const CellInfo& cell, const std::vector<Coordinate>& coordinates){

    QString html;

    html += R"( <p> Полученные данные из log-файла для рассчетов значений ветра: </p> )";

    const Coordinate* closest = nullptr;
    double minDiff = std::numeric_limits<double>::max();

    for (const Coordinate& item : coordinates)
    {
        double diff = std::abs(item.H - cell.height);

        if (diff < minDiff)
        {
            minDiff = diff;
            closest = &item;
        }
    }

    if (closest)
    {
        html += QString("<p>дальность dglob на высоте %1 м: %2</p>")
                    .arg(closest->H)
                    .arg(closest->dglob);

        html += QString("<p>азимут aglob на высоте %1 м: %2</p>")
                    .arg(closest->H)
                    .arg(closest->aglob);

        html += QString("<p>угол места eglob на высоте %1 м: %2</p>")
                    .arg(closest->H)
                    .arg(closest->eglob);

        html += QString("<p>время, через которое зонд был на высоте %1 м: %2</p>")
                    .arg(closest->H)
                    .arg(closest->S);
    }

    html += R"(

<p>
При сопровождении радиозонда вычисляются текущие значения прямоугольных координат
<b>X<sub>j</sub></b>, <b>Z<sub>j</sub></b> и высота <b>H<sub>j</sub></b>,
определяющие положение радиозонда по сглаженным значениям координат
<b>D<sub>j</sub></b>, <b>E<sub>j</sub></b>, <b>A<sub>j</sub></b>.
</p>
)";

    html += "<p style='text-align:center;'><img src=':/images/formula1.png' width='400'></p>";

    auto it = std::min_element(coordinates.begin(), coordinates.end(),
                               [&](const Coordinate& a, const Coordinate& b)
                               {
                                   return std::abs(a.H - cell.height) < std::abs(b.H - cell.height);
                               });

    if (it != coordinates.end())
    {
        html += QString("<p>значение X на высоте %1 м: %2</p>")
                    .arg(it->H)
                    .arg(it->X);

        html += QString("<p>значение Z на высоте %1 м: %2</p>")
                    .arg(it->H)
                    .arg(it->Z);
    }

    return html;
}


QString DisplayManager::HtmlMtdV(const CellInfo& cell,
                              const std::vector<Zone>& zones,
                              const std::vector<Mtd>& mtd, const std::vector<Coordinate>& coordinates)
{
    Zone currentZone{};
    Mtd currentH{};

    // ищу текущую зону
    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 0.001){
            currentZone = item;
            break;
        }
    }

    for (const Mtd& item : mtd){
        if (std::abs(item.h - cell.height) < 0.001){
            currentH = item;
            break;
        }
    }

    QString html;

    //делаем заголовок
    html += generateHeader(cell);

    //обработка логов, вычисление координат
    html += HtmlCoordinates(cell, coordinates);

    html += R"(
<p>
Из последовательности прямоугольных координат выбираются координаты,
являющиеся границами зон (по высоте):
</p>

<ul>
<li>100 м — до 500 м</li>
<li>200 м — от 500 м до 6 км</li>
<li>400 м — от 6 км до 14 км</li>
<li>800 м — выше 14 км</li>
</ul>

<p>
Для каждой k-ой зоны слоя вычисляются составляющие ветра:
</p>
)";

    html += "<p style='text-align:center;'><img src=':/images/formula2.png' width='250'></p>";
    html += "<p style='text-align:center;'><img src=':/images/formula3.png' width='250'></p>";

    html += QString("<p>значение Vx в зоне %1 м: %2</p>")
                .arg(currentZone.height)
                .arg(currentZone.vx);

    html += QString("<p>значение Vz в зоне %1 м: %2</p>")
                .arg(currentZone.height)
                .arg(currentZone.vz);

    html += R"(
<p>
Искомые значения Vxi, Vzi рассчитываются путем линейной интерполяции
с использованием массива значений Vxk, Vzk.
</p>
)";

    html += "<p style='text-align:center;'><img src=':/images/formula6.png' width='250'></p>";

    html += QString("<p>значение Vx на высоте %1 м: %2</p>")
                .arg(currentH.h)
                .arg(currentH.vx);

    html += QString("<p>значение Vz на высоте %1 м: %2</p>")
                .arg(currentH.h)
                .arg(currentH.vz);

    html += R"(

<p>
Вычисление скорости действительного ветра
на стандартной высоте производится по формуле:
</p>

)";

    html += "<p style='text-align:center;'><img src=':/images/formula7.png' width='250'></p>";

    html += QString("<p>значение скорости действительного ветра v на высоте %1 м: %2</p>")
                .arg(currentH.h)
                .arg(currentH.v);

    return html;
}

QString DisplayManager::HtmlMtdAV(const CellInfo& cell,
                               const std::vector<Zone>& zones,
                               const std::vector<Mtd>& mtd, const std::vector<Coordinate>& coordinates)
{
    QString html;

    html += HtmlMtdV(cell, zones, mtd, coordinates);

    html += R"(

<p>
Вычисление направления действительного ветра
на стандартной высоте производится по формуле:
</p>
)";
    html += "<p style='text-align:center;'><img src=':/images/formula8.png' width='400'></p>";

    for (const Mtd& item : mtd)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            html += QString("<p>значение направления действительного ветра av на высоте %1 м: %2</p>")
                        .arg(item.h)
                        .arg(item.av);
            break;
        }
    }

    return html;
}

QString DisplayManager::HtmlMtsV(const CellInfo& cell,
                                 const std::vector<Zone>& zones,
                                 const std::vector<Mts>& mts, const std::vector<Coordinate>& coordinates){
    Zone currentZone{};
    Mts currentH{};

    // ищу текущую зону
    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 0.001){
            currentZone = item;
            break;
        }
    }

    for (const Mts& item : mts){
        if (std::abs(item.h - cell.height) < 0.001){
            currentH = item;
            break;
        }
    }

    QString html;

    //делаем заголовок
    html += generateHeader(cell);

    //обработка логов, вычисление координат
    html += HtmlCoordinates(cell, coordinates);

    html += R"(
<p>
Из последовательности прямоугольных координат выбираются координаты,
являющиеся границами зон (по высоте):
</p>

<ul>
<li>100 м — до 500 м</li>
<li>200 м — от 500 м до 6 км</li>
<li>400 м — от 6 км до 14 км</li>
<li>800 м — выше 14 км</li>
</ul>

<p>
Вычисление скорости и направления среднего ветра в слое производится по формулам:
</p>
)";
    return html;

}


QString DisplayManager::HtmlMtsAV(const CellInfo& cell,
                                  const std::vector<Zone>& zones,
                                  const std::vector<Mts>& mts, const std::vector<Coordinate>& coordinates)
{
    QString html;

    html += HtmlMtsV(cell, zones, mts, coordinates);

    html += R"(

<p>
Вычисление направления действительного ветра
на стандартной высоте производится по формуле:
</p>
)";
    html += "<p style='text-align:center;'><img src=':/images/formula8.png' width='400'></p>";

    for (const Mts& item : mts)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            html += QString("<p>значение направления действительного ветра av на высоте %1 м: %2</p>")
                        .arg(item.h)
                        .arg(item.aw);
            break;
        }
    }

    return html;
}


// рассчеты температуры для зон (без бюллетеней)
QString DisplayManager::HtmlTTi(const CellInfo& cell,
                                const std::vector<Zone>& zones,
                                const UserConstants& constants, std::vector<TemperatureRecord>& records){

    Zone currentZone{};

    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 0.001){
            currentZone = item;
            break;
        }
    }

    QString html;
    html += R"(
<p>
Расчет температуры производится вне зависимости от режима работы изделия. Температура рассчитывается по формулам:
</p>
)";
    html += QString("<p>Константы, введенные пользователем: %1 %2 %3 %4 %5</p>")
                .arg(constants.A)
                .arg(constants.B)
                .arg(constants.C)
                .arg(constants.R1)
                .arg(constants.R2);

    html += R"(
<p>
и относится к середине канального интервала.
Обозначения в формулах:
A,B,C - коэффициенты термистора радиозонда;
R1, R2 - коэффициенты радиоблока радиозонда;
QO, QT - длительности периодов опорной и температурной метеочастот телеметрического сигнала радиозонда.
</p>

<p> Вычисление действительной температуры производится в промежуточных слоях между границами стандартных слоев по формуле:
</p>
)";
    html += QString("<p>значение Tn в зоне %1 м: %2</p>")
                .arg(currentZone.height)
                .arg(currentZone.Tn);

    html += R"(
<p>
где Tk - температура, измеренная в текущем канальном интервале сигнала радиозонда;
n - количество измерений температуры в промежуточном слое
</p>
)";
    html += R"(
<p>
Полученная температура относится к высоте середины слоя, которую обозначим как Hi.
            Затем производится коррекция температуры Tni на радиационную поправку дельтаTp,
            зависящую от высоты подъема радиозонда над уровнем моря, высоты стояния Солнца и температуры воздуха:
</p>
)";
    html += R"(
<p>
где дельта Tp - радиационная поправка, определяемая метедом линейной интерполяции по двухвходовой таблице радиационных поправок.
</p>
)";

    html += R"(
<p>
Если U0 было введено, то значение дельта Tвр рассчитывается по формуле:
</p>
)";
    html += QString("<p>Значения приземных измерений U0, T0, P0 равны соответственно: %1 %2 %3</p>")
                .arg(constants.U0)
                .arg(constants.T0)
                .arg(constants.P0);

    html += QString("<p>Виртуальная поправка в зоне %1 равна %2, высота середины зоны Hi равна %3</p>")
                .arg(currentZone.height)
                .arg(currentZone.dTvir)
                .arg(currentZone.Hi);

    html += R"(
<p>
Температура на высоте середины зоны рассчитывается по формуле:
</p>
)";

    html += QString("<p>Температура с учетом виртуальной поправки на высоте середины зоны %1 равна %2</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Tvrn);

    html += R"(
<p>
Отклонение температуры от табличной в слое дельтаH = Hm - Hm-1 и среднее отклонение температуры в слое (0, Hm) определяются по формулам:
</p>
)";

    html += QString("<p>Табличная температура на высоте %1 равна %2. Отклонение температуры от табличной на высоте %1 равно %3."
                    "Среднее отклонение от температуры на высоте %1 равно %4</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Ttab)
                .arg(currentZone.TTi)
                .arg(currentZone.TTcpm);

    return html;
}

//линейная интерполяция температуры для mtd
QString DisplayManager::HtmlTTiMtd(const CellInfo& cell,
                                const std::vector<Zone>& zones,
                                const std::vector<Mtd>& mtd, const UserConstants& constants, std::vector<TemperatureRecord>& records)
{
    QString html;
    html += HtmlTTi(cell, zones, constants, records);

    for (const Mtd& item : mtd)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            html += QString("<p>Значение отклонения температуры от табличной на стандартной высоте бюллетеня \"Метеодействительный\" рассчитывается"
                            "с помощью линейной интерполяции.</p> <p>Отклонение температуры от табличной на высоте %1 : %2</p> "
                            "<p>Среднее отклонение температуры от табличной на высоте %1 : %3</p>")
                        .arg(item.h)
                        .arg(item.TTi)
                        .arg(item.TTcpm);
            break;
        }
    }
    return html;
}

QString DisplayManager::HtmlTTiMts(const CellInfo& cell,
                                const std::vector<Zone>& zones,
                                const std::vector<Mts>& mts, const UserConstants& constants, std::vector<TemperatureRecord>& records)
{
    QString html;
    html += HtmlTTi(cell, zones, constants, records);

    for (const Mts& item : mts)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            html += QString("<p>Значение отклонения температуры от табличной на стандартной высоте бюллетеня \"Метеосредний\" рассчитывается"
                            "с помощью линейной интерполяции.</p> <p>Отклонение температуры от табличной на высоте %1 : %2</p> "
                            "<p>Среднее отклонение температуры от табличной на высоте %1 : %3</p>")
                        .arg(item.h)
                        .arg(item.TTi)
                        .arg(item.TTcpm);
            break;
        }
    }
    return html;
}

QString DisplayManager::HtmlPPi(const CellInfo& cell, const std::vector<Zone>& zones){
    QString html;
    html += R"(
<p>
Плотностные характеристики атмосферы определяются для тех же стандартных слоев, что и температурные, до высоты 10 км.
</p>

<p> Вычисление плотности в слое (Hm-1, Hm) производится следующим образом: определяется давление на середине стандартного слоя по формуле:</p>
)";

    for (const Zone& item : zones)
        {
            if (std::abs(item.height - cell.height) < 0.001)
            {
                html += QString("<p>Плотность в слое %1 равна %2.</p>")
                            .arg(item.height)
                            .arg(item.Pn);
                break;
            }
        }


    html += R"(
<p>
Плотность воздуха Пi определяется для середины слоя, используя значение давления Pni в мм. рт. ст.
</p>
)";

    for (const Zone& item : zones)
        {
            if (std::abs(item.height - cell.height) < 0.001)
            {
                html += QString("<p>Плотность воздуха Пi на высоте %1 равна %2</p>")
                            .arg(item.Hi)
                            .arg(item.Pi);
                break;
            }
        }

    html += R"(
<p>
Расчет действительного отклонения плотности ППi и среднего отклонения плотности ППcpm
            от табличного значения производится в середине слоя по формуле:
</p>
)";

    return html;

}
