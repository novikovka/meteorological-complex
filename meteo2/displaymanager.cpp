#include "displaymanager.h"
#include <cmath>

DisplayManager::DisplayManager()
{
}

QString DisplayManager::HtmlV(const CellInfo& cell,
                              const std::vector<Zone>& zones,
                              const std::vector<Mtd>& mtd)
{
    QString html;

    html += R"(

<p>
При сопровождении радиозонда вычисляются текущие значения прямоугольных координат
<b>X<sub>j</sub></b>, <b>Z<sub>j</sub></b> и высота <b>H<sub>j</sub></b>,
определяющие положение радиозонда по сглаженным значениям координат
<b>D<sub>j</sub></b>, <b>E<sub>j</sub></b>, <b>A<sub>j</sub></b>.
</p>
)";

    html += "<p style='text-align:center;'><img src=':/images/formula1.png' width='400'></p>";

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

    for (const Zone& item : zones)
    {
        if (std::abs(item.height - cell.height) < 0.001)
        {
            html += QString("<p>значение Vx в зоне %1 м: %2</p>")
                        .arg(item.height)
                        .arg(item.vx);

            html += QString("<p>значение Vz в зоне %1 м: %2</p>")
                        .arg(item.height)
                        .arg(item.vz);

            break;
        }
    }

    html += R"(
<p>
Искомые значения Vxi, Vzi рассчитываются путем линейной интерполяции
с использованием массива значений Vxk, Vzk.
</p>
)";

    html += "<p style='text-align:center;'><img src=':/images/formula6.png' width='250'></p>";

    for (const Mtd& item : mtd)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            html += QString("<p>значение Vx на высоте %1 м: %2</p>")
                        .arg(item.h)
                        .arg(item.vx);

            html += QString("<p>значение Vz на высоте %1 м: %2</p>")
                        .arg(item.h)
                        .arg(item.vz);

            break;
        }
    }

    html += R"(

<p>
Вычисление скорости действительного ветра
на стандартной высоте производится по формуле:
</p>

)";

    html += "<p style='text-align:center;'><img src=':/images/formula7.png' width='250'></p>";

    for (const Mtd& item : mtd)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            html += QString("<p>значение скорости действительного ветра v на высоте %1 м: %2</p>")
                        .arg(item.h)
                        .arg(item.v);
            break;
        }
    }

    return html;
}

QString DisplayManager::HtmlAV(const CellInfo& cell,
                               const std::vector<Zone>& zones,
                               const std::vector<Mtd>& mtd)
{
    QString html;

    html += HtmlV(cell, zones, mtd);

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

QString DisplayManager::generateHtml(const CellInfo& cell,
                                     const std::vector<Zone>& zones,
                                     const std::vector<Mtd>& mtd)
{
    QString html;

    QString type_bull;

    if (cell.tableName == "TableMtd")
        type_bull = "Метеодействительный";
    else if (cell.tableName == "TableMts")
        type_bull = "Метеосредний";
    else
        type_bull = "Неизвестно";

    QString paramName;

    if (cell.columnName == "v")
        paramName = "v — скорость";
    else
        paramName = cell.columnName;

    html += "<h2>Информация о параметре</h2>";

    html += QString("<p><b>Бюллетень:</b> %1</p>").arg(type_bull);
    html += QString("<p><b>Высота:</b> %1 метров</p>").arg(cell.height);
    html += QString("<p><b>Параметр:</b> %1</p>").arg(paramName);
    html += QString("<p><b>Значение:</b> %1 м/с</p>").arg(cell.cellValue);

    html += "<hr>";
    html += "<h3>Последовательность расчетов</h3>";

    if(cell.columnName == "v"){
        html += HtmlV(cell, zones, mtd);
    }else if (cell.columnName == "av"){
        html += HtmlAV(cell, zones, mtd);
    }

    return html;
}

QString DisplayManager::generateHtmlMts(const CellInfo& cell,
                                     const std::vector<Zone>& zones,
                                     const std::vector<Mts>& mts)
{
    QString html;

    QString type_bull;

    if (cell.tableName == "TableMtd")
        type_bull = "Метеодействительный";
    else if (cell.tableName == "TableMts")
        type_bull = "Метеосредний";
    else
        type_bull = "Неизвестно";

    QString paramName;

    if (cell.columnName == "w")
        paramName = "w — скорость";
    else
        paramName = cell.columnName;

    html += "<h2>Информация о параметре</h2>";

    html += QString("<p><b>Бюллетень:</b> %1</p>").arg(type_bull);
    html += QString("<p><b>Высота:</b> %1 метров</p>").arg(cell.height);
    html += QString("<p><b>Параметр:</b> %1</p>").arg(paramName);
    html += QString("<p><b>Значение:</b> %1 м/с</p>").arg(cell.cellValue);

    html += "<hr>";
    html += "<h3>Последовательность расчетов</h3>";

    return html;
}

QString DisplayManager::HtmlTTi(const CellInfo& cell,
                                const std::vector<Zone>& zones,
                                const std::vector<Mtd>& mtd)
{
    Q_UNUSED(cell);
    Q_UNUSED(zones);
    Q_UNUSED(mtd);

    return "<p>Расчет параметра TTi пока не реализован.</p>";
}
