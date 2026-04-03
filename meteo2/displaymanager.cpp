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

    html += QString("<p> Полученные данные из log-файла для рассчетов значений ветра на высоте %1 м: </p>")
                .arg(cell.height);

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
        html += QString("<p>Дальность (dglob): <b>%1</b></p>")
                    .arg(closest->dglob);

        html += QString("<p>Азимут (aglob): <b>%1</b></p>")
                    .arg(closest->aglob);

        html += QString("<p>Угол места (eglob): <b>%1</b></p>")
                    .arg(closest->eglob);

        html += QString("<p>Время (S), через которое зонд был на выбранной высоте (S):<b>%1</b></p>")
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
        html += QString("<p>значение <b>X</b> на высоте %1 м: <b>%2</b></p>")
                    .arg(it->H)
                    .arg(it->X);

        html += QString("<p>значение <b>Z</b> на высоте %1 м: <b>%2</b></p>")
                    .arg(it->H)
                    .arg(it->Z);
    }

    return html;
}

QString DisplayManager::HtmlMtdV(const CellInfo& cell, const Zone& currentZone,
                              const std::vector<Mtd>& mtd, const std::vector<Coordinate>& coordinates)
{
    //Zone currentZone{};
    Mtd currentH{};

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

    html += "<p style='text-align:center;'><img src=':/images/formula2.png' width='200'></p>";
    html += "<p style='text-align:center;'><img src=':/images/formula3.png' width='200'></p>";
    html += "<p style='text-align:center;'><img src=':/images/formula5.png' width='200'></p>";

    html += QString("<p>значение <b>Vxk</b> в зоне %1 м: <b>%2</b></p>")
                .arg(currentZone.height)
                .arg(currentZone.vx);

    html += QString("<p>значение <b>Vzk</b> в зоне %1 м: <b>%2</b></p>")
                .arg(currentZone.height)
                .arg(currentZone.vz);
    html += QString("<p>значение <b>Yk</b> в зоне %1 м: <b>%2</b></p>")
                .arg(currentZone.height)
                .arg(currentZone.y);

    html += R"(
<p>
Искомые значения Vxi, Vzi для высот бюллетеня рассчитываются путем линейной интерполяции
с использованием массива значений Vxk, Vzk.
</p>
)";

    html += "<p style='text-align:center;'><img src=':/images/formula6.png' width='350'></p>";

    html += QString("<p>значение <b>Vxi</b> на высоте %1 м: <b>%2</b></p>")
                .arg(currentH.h)
                .arg(currentH.vx);

    html += QString("<p>значение <b>Vzi</b> на высоте %1 м: <b>%2</b></p>")
                .arg(currentH.h)
                .arg(currentH.vz);

    html += R"(

<p>
Вычисление скорости действительного ветра
на стандартной высоте производится по формуле:
</p>

)";

    html += "<p style='text-align:center;'><img src=':/images/formula7.png' width='250'></p>";

    html += QString("<p>значение скорости действительного ветра <b>v</b> на высоте %1 м: <b>%2</b></p>")
                .arg(currentH.h)
                .arg(currentH.v);

    return html;
}

QString DisplayManager::HtmlMtdAV(const CellInfo& cell,
                               const Zone& current_zone,
                               const std::vector<Mtd>& mtd, const std::vector<Coordinate>& coordinates)
{
    QString html;

    html += HtmlMtdV(cell, current_zone, mtd, coordinates);

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

QString DisplayManager::HtmlTTi(const CellInfo& cell,
                                const std::vector<Zone>& zones,
                                const UserConstants& constants)
{
    Zone currentZone{};

    for (const Zone& item : zones)
    {
        if (std::abs(item.height - cell.height) < 0.001)
        {
            currentZone = item;
            break;
        }
    }

    QString html;

    html += generateHeader(cell);

    html += R"(
            <p>
            Расчет температуры производится вне зависимости от режима работы изделия.
            Температура рассчитывается по формулам:
            </p>

            <p style='text-align:center;'><img src=":/TT/TTi(1).png" width="400"></p>

            <div style="display:flex; justify-content:center; gap:100px;">
                <p style='text-align:center;'><img src=":/TT/TTi(2).png" width="195"></p>
                <p style='text-align:center;'><img src=":/TT/TTi(3).png" width="150"></p>
            </div>

            <p>
            и относится к середине канального интервала.
            </p>

            <p>
            Обозначения в формулах:
            </p>

            <ul>
            <li>A, B, C — коэффициенты термистора радиозонда</li>
            <li>R1, R2 — коэффициенты радиоблока радиозонда</li>
            <li>QO, QT — длительности периодов опорной и температурной метеочастот телеметрического сигнала</li>
            </ul>
            )";



    html += QString(
                R"(
<p class="block">
Константы, введенные пользователем:
</p>

<ul>
<li>A = <span class="value">%1</span></li>
<li>B = <span class="value">%2</span></li>
<li>C = <span class="value">%3</span></li>
<li>R1 = <span class="value">%4</span></li>
<li>R2 = <span class="value">%5</span></li>
</ul>
)")
                .arg(constants.A)
                .arg(constants.B)
                .arg(constants.C)
                .arg(constants.R1)
                .arg(constants.R2);

    html += R"(

<p class="block">
Вычисление действительной температуры производится в промежуточных слоях между
границами стандартных слоев по формуле:
</p>

<p style='text-align:center;'> <img src=":/TT/TTi(4).png" width="250"> </p>

<p>
где:
</p>

<ul>
<li>Tk — температура в текущем канальном интервале</li>
<li>n — количество измерений температуры в промежуточном слое</li>
</ul>

)";

    html += QString(
                "<p>Значение <b>Tn</b> в зоне <b><span class='value'>%1 м</span></b> равно "
                "<span class='value'><b>%2</b></span></p>")
                .arg(currentZone.height)
                .arg(currentZone.Tn);

    html += R"(

<p class="block">
Полученная температура относится к высоте середины слоя, обозначенной <b>Hi</b>.
</p>

<p>
Далее производится коррекция температуры на радиационную поправку:
</p>

<p style='text-align:center;'><img src=":/TT/TTi(5).png" width="350"></p>


<p>
где <b>ΔTp</b> определяется методом линейной интерполяции
по двухвходовой таблице радиационных поправок.
</p>

)";

    html += R"(

<p class="block">
Если <b>U<sub>0</sub></b> было введено, виртуальная поправка вычисляется по формуле:
</p>

<p style='text-align:center;'><img src=":/TT/TTi(6).png" width="350"></p>

)";

    html += QString(
                "<p>Приземные измерения: "
                "U<sub>0</sub> = <span class='value'>%1</span>, "
                "T<sub>0</sub> = <span class='value'>%2</span>, "
                "P<sub>0</sub> = <span class='value'>%3</span></p>")
                .arg(constants.U0)
                .arg(constants.T0)
                .arg(constants.P0);

    html += QString(
                "<p></b>Виртуальная поправка в зоне </b>"
                "<b><span class='value'>%1</span> равна </b>"
                "<b><span class='value'>%2</span>, </b>"
                "высота середины слоя H<sub>i</sub> = "
                "<span class='value'>%3</span></p>")
                .arg(currentZone.height)
                .arg(currentZone.dTvir)
                .arg(currentZone.Hi);

    html += R"(

<p class="block">
Температура на высоте середины зоны:
</p>

<p style='text-align:center;'><img src=":/TT/TTi(7).png" width="600"></p>

)";

    html += QString(
                "<p><b>Температура с учетом виртуальной поправки </b>"
                "<b>на высоте <span class='value'>%1</span> равна </b>"
                "<b><span class='value'>%2</span></p></b>")
                .arg(currentZone.Hi)
                .arg(currentZone.Tvrn);

    html += R"(

<p class="block">
Отклонение температуры от табличной в слое
<b>ΔH = H<sub>m</sub> − H<sub>m-1</sub> </b>определяется по формуле:
</p>

<p style='text-align:center;'> <img src=":/TT/TTi(8).png" width="350"> </p>


)";

    html += QString(
                "<p>Табличная температура на высоте "
                "<span class='value'>%1</span> равна "
                "<span class='value'><b>%2</b></span>.<br>"
                "Отклонение температуры от табличной равно "
                "<span class='value'><b>%3</b></span>.</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Ttab)
                .arg(currentZone.TTi);

    return html;
}

QString DisplayManager::HtmlTTcpm(const CellInfo& cell,
                                  const std::vector<Zone>& zones,
                                  const UserConstants& constants){

    Zone currentZone{};

    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 0.001){
            currentZone = item;
            break;
        }
    }

    QString html;
    html += HtmlTTi(cell, zones, constants);

    html += R"(
<p>
Среднее отклонение температуры в слое ΔH = H<sub>m</sub> - H<sub>m-1</sub> в слое (0, Hm) определяется по формуле:
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/TT/TTi(9).png" width="400">
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

QString DisplayManager::HtmlTTcpmMtd(const CellInfo& cell,const std::vector<Zone>& zones, const std::vector<Mtd>& mtd,
                                     const UserConstants& constants){
    QString html;
    html+= HtmlTTcpm(cell, zones, constants);

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

QString DisplayManager::HtmlTTcpmMts(const CellInfo& cell,const std::vector<Zone>& zones, const std::vector<Mts>& mts,
                                     const UserConstants& constants){
    QString html;
    html+= HtmlTTcpm(cell, zones, constants);

    for (const Mts& item : mts)
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

QString DisplayManager::HtmlTTiMtd(const CellInfo& cell,
                                const std::vector<Zone>& zones,
                                const std::vector<Mtd>& mtd, const UserConstants& constants)
{
    QString html;
    html += HtmlTTi(cell, zones, constants);

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
                                const std::vector<Mts>& mts, const UserConstants& constants)
{
    QString html;
    html += HtmlTTi(cell, zones, constants);

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

    html += generateHeader(cell);
    html += R"(
<p>
Плотностные характеристики атмосферы определяются для тех же стандартных слоев, что и температурные, до высоты 10 км.
</p>

<p> Вычисление плотности в слое (Hm-1, Hm) производится следующим образом: определяется давление на середине стандартного слоя по формуле:</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/P/P(1).png" width="500">
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/P/P(2).png" width="400">
</p>
)";
    Zone currentZone{};

    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 0.001){
                currentZone = item;
                break;
            }
        }

    html += QString("<p>Плотность в слое %1 равна %2.</p>")
                .arg(currentZone.height)
                .arg(currentZone.Pn);

    html += R"(
<p>
Плотность воздуха Пi определяется для середины слоя, используя значение давления Pni в мм. рт. ст.
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/P/P(3).png" width="250">
</p>
)";

    html += QString("<p>Плотность воздуха Пi на высоте %1 равна %2</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Pi);

    html += R"(
<p>
Расчет действительного отклонения плотности ППi и среднего отклонения плотности ППcpm
            от табличного значения производится в середине слоя по формуле:
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/P/P(4).png" width="300">
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/P/P(5).png" width="400">
</p>
)";

    return html;

}


QString DisplayManager::HtmlPPiMtd(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd){
    QString html;
    Mtd current_H{};

    for (const Mtd& item : mtd)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            current_H = item;
            break;
        }
    }

    html += HtmlPPi(cell, zones);

    html += QString("<p>Отклонение плотности ППi от табличного значения на высоте %1 : ППi = %2</p>")
                .arg(current_H.h)
                .arg(current_H.PPi);

    return html;
}

QString DisplayManager::HtmlPPiMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts){
    QString html;
    Mts current_H{};

    for (const Mts& item : mts)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            current_H = item;
            break;
        }
    }

    html += HtmlPPi(cell, zones);

    html += QString("<p>Отклонение плотности ППi от табличного значения на высоте %1 : ППi = %2</p>")
                .arg(current_H.h)
                .arg(current_H.PPi);

    return html;
}

QString DisplayManager::HtmlPPcpmMtd(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mtd>& mtd){
    QString html;
    Mtd current_H{};

    for (const Mtd& item : mtd)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            current_H = item;
            break;
        }
    }

    html += HtmlPPi(cell, zones);

    html += QString("<p>Среднее отклонение плотности ППi от табличного значения на высоте %1 м: ППcpm = %2</p>")
                .arg(current_H.h)
                .arg(current_H.PPcpm);

    return html;
}

QString DisplayManager::HtmlPPcpmMts(const CellInfo& cell, const std::vector<Zone>& zones, const std::vector<Mts>& mts){
    QString html;
    Mts current_H{};

    for (const Mts& item : mts)
    {
        if (std::abs(item.h - cell.height) < 0.001)
        {
            current_H = item;
            break;
        }
    }

    html += HtmlPPi(cell, zones);

    html += QString("<p>Среднее отклонение плотности ППcpm от табличного значения на высоте %1 м: ППcpm = %2</p>")
                .arg(current_H.h)
                .arg(current_H.PPcpm);

    return html;
}
