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
                                 const std::vector<Mtd>& mtd,
                                 const std::vector<Coordinate>& coordinates)
{
    Zone currentZone{};
    Mtd currentH{};

    for (const Zone& z : zones)
        if (std::abs(z.height - cell.height) < 0.001)
            currentZone = z;

    for (const Mtd& h : mtd)
        if (std::abs(h.h - cell.height) < 0.001)
            currentH = h;

    QString html;

    html += generateHeader(cell);
    html += HtmlCoordinates(cell, coordinates);

    QString block = R"(

<h3>Выбор зон по высоте</h3>

<p>
Из последовательности прямоугольных координат выбираются координаты,
являющиеся границами зон:
</p>

<table border="1" cellpadding="6" cellspacing="0">
<tr>
<th>Шаг</th>
<th>Диапазон высот</th>
</tr>
<tr><td>100 м</td><td>до 500 м</td></tr>
<tr><td>200 м</td><td>500 м – 6 км</td></tr>
<tr><td>400 м</td><td>6 км – 14 км</td></tr>
<tr><td>800 м</td><td>выше 14 км</td></tr>
</table>

<h3>Составляющие ветра</h3>

<p>Для каждой зоны вычисляются компоненты ветра:</p>

<div align="center">
<img src=":/images/formula2.png" width="220"><br>
<img src=":/images/formula3.png" width="220">
</div>

<p>
<b>Зона %1 м</b>
</p>

<ul>
<li>Vx = %2</li>
<li>Vz = %3</li>
</ul>

<h3>Линейная интерполяция</h3>

<p>
Значения Vx и Vz на промежуточной высоте вычисляются методом линейной
интерполяции:
</p>

<div align="center">
<img src=":/images/formula6.png" width="220">
</div>

<p>
<b>Высота %4 м</b>
</p>

<ul>
<li>Vx = %5</li>
<li>Vz = %6</li>
</ul>

<h3>Скорость действительного ветра</h3>

<p>Скорость ветра вычисляется по формуле:</p>

<div align="center">
<img src=":/images/formula7.png" width="220">
</div>

<p>
<b>Скорость ветра на высоте %7 м:</b> %8 м/с
</p>

)";

    html += block
                .arg(currentZone.height)
                .arg(currentZone.vx)
                .arg(currentZone.vz)
                .arg(currentH.h)
                .arg(currentH.vx)
                .arg(currentH.vz)
                .arg(currentH.h)
                .arg(currentH.v);

    return html;
}

/*
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

*/

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

/*
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
*/

QString DisplayManager::HtmlMtsV(const CellInfo& cell,
                                 const std::vector<Zone>& zones,
                                 const std::vector<Mts>& mts,
                                 const std::vector<Coordinate>& coordinates)
{
    Zone currentZone{};
    Mts currentH{};

    // поиск текущей зоны
    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 0.001){
            currentZone = item;
            break;
        }
    }

    // поиск текущего слоя МТС
    for (const Mts& item : mts){
        if (std::abs(item.h - cell.height) < 0.001){
            currentH = item;
            break;
        }
    }

    QString html;

    // заголовок
    html += generateHeader(cell);

    // координаты
    html += HtmlCoordinates(cell, coordinates);

    html += R"(

<p>
Из последовательности прямоугольных координат выбираются координаты,
являющиеся границами зон по высоте подъема радиозонда.
</p>

<p>
Границы зон принимаются в зависимости от высоты:
</p>

<ul>
<li>100 м — до 500 м</li>
<li>200 м — от 500 м до 6 км</li>
<li>400 м — от 6 км до 14 км</li>
<li>800 м — выше 14 км</li>
</ul>

<p>
По координатам, соответствующим границам зон, вычисляются приращения
прямоугольных координат в слое:
</p>

<p style="text-align:center;">
<img src=":/MTS/MtsV(1).png" width="350">
</p>

<p>
где ΔX и ΔY — изменения координат радиозонда в пределах слоя.
</p>

<p>
Средняя скорость ветра в слое рассчитывается по формуле:
</p>

<p style="text-align:center;">
<img src=":/MTS/MtsV(2).png" width="350">
</p>

<p>
где:
</p>

<ul>
<li>V — скорость среднего ветра в слое</li>
<li>ΔX, ΔY — приращения координат</li>
<li>Δt — время прохождения слоя</li>
</ul>

<p>
Направление среднего ветра определяется по формуле:
</p>

<p style="text-align:center;">
<img src=":/MTS/MtsV(3).png" width="350">
</p>

<p>
Полученное направление ветра отсчитывается в градусах относительно
северного направления по часовой стрелке.
</p>

)";

    html += QString(
                "<p>Для слоя с высотой %1 м получены следующие параметры:</p>"
                ).arg(currentZone.height);

    html += QString(
                "<p>Средняя скорость ветра V = %1 м/с</p>"
                ).arg(currentH.w);

    html += QString(
                "<p>Направление ветра D = %1°</p>"
                ).arg(currentH.aw);

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
/*
QString DisplayManager::HtmlTTi(const CellInfo& cell,
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

    html += generateHeader(cell);

    html += R"(
<p>
Расчет температуры производится вне зависимости от режима работы изделия. Температура рассчитывается по формулам:
</p>
)";
    html += R"(

<p style="text-align:center;">
    <img src=":/TT/TTi(1).png" width="400">
</p>

<div style="display:flex; justify-content:center; gap:10px;">
    <img src=":/TT/TTi(2).png" width="195">
    <img src=":/TT/TTi(3).png" width="195">
</div>
)";
    html += R"(
<p>
и относится к середине канального интервала.
Обозначения в формулах:
A,B,C - коэффициенты термистора радиозонда;
R1, R2 - коэффициенты радиоблока радиозонда;
QO, QT - длительности периодов опорной и температурной метеочастот телеметрического сигнала радиозонда.
</p>

)";

    html += QString("<p>Константы, введенные пользователем: A = %1, B = %2, C = %3, R1 = %4, R2 = %5</p>")
                .arg(constants.A)
                .arg(constants.B)
                .arg(constants.C)
                .arg(constants.R1)
                .arg(constants.R2);

    html += R"(
<p> Вычисление действительной температуры производится в промежуточных слоях между границами стандартных слоев по формуле:
</p>
)";

    html += R"(
<p style="text-align:center;">
    <img src=":/TT/TTi(4).png" width="400">
</p>
<p>
где Tk - температура, измеренная в текущем канальном интервале сигнала радиозонда;
n - количество измерений температуры в промежуточном слое
</p>

)";
    html += QString("<p>значение Tn в зоне %1 м: %2</p>")
                .arg(currentZone.height)
                .arg(currentZone.Tn);

    html += R"(
<p>
Полученная температура относится к высоте середины слоя, которую обозначим как Hi.
</p>
<p>

            Затем производится коррекция температуры Tni на радиационную поправку дельтаTp,
            зависящую от высоты подъема радиозонда над уровнем моря, высоты стояния Солнца и температуры воздуха:
</p>

)";
    html += R"(
<p style="text-align:center;">
    <img src=":/TT/TTi(5).png" width="400">
</p>
<p>
где дельта Tp - радиационная поправка, определяемая метедом линейной интерполяции по двухвходовой таблице радиационных поправок.
</p>

)";

    html += R"(
<p>
Если U<sub>0</sub> было введено, то значение &Delta;T<sub>вр</sub> рассчитывается по формуле:
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/TT/TTi(6).png" width="400">
</p>
)";

    html += QString("<p>Значения приземных измерений U0, T0, P0 равны соответственно: %1, %2, %3</p>")
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
    html += R"(
<p style="text-align:center;">
    <img src=":/TT/TTi(7).png" width="400">
</p>
)";

    html += QString("<p>Температура с учетом виртуальной поправки на высоте середины зоны %1 равна %2</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Tvrn);

    html += R"(
<p>
Отклонение температуры от табличной в слое дельтаH = Hm - Hm-1 в слое (0, Hm) определяется по формуле:
</p>
)";
    html += R"(
<p style="text-align:center;">
    <img src=":/TT/TTi(8).png" width="400">
</p>
)";

    html += QString("<p>Табличная температура на высоте %1 равна %2. Отклонение температуры от табличной на высоте %1 равно %3.</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Ttab)
                .arg(currentZone.TTi);

    return html;
}
*/

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

<style>
body{
    font-family: "Segoe UI", Arial;
    line-height: 1.5;
}

.container{
    max-width: 850px;
    margin: auto;
}

.formula{
    text-align:center;
    margin:20px 0;
}

.formula img{
    max-width:400px;
}

.formula-row{
    display:flex;
    justify-content:center;
    align-items:center;
    gap:10px;
    margin:20px 0;
}

.formula-row img{
    width:150px;

}

.block{
    margin-top:18px;
}

.value{
    font-weight:600;
    color:#003366;
}

</style>

<div class="container">

<p>
Расчет температуры производится вне зависимости от режима работы изделия.
Температура рассчитывается по формулам:
</p>

<div class="formula">
<img src=":/TT/TTi(1).png">
</div>

<div style="display:flex; justify-content:center; gap:10px;">
    <img src=":/TT/TTi(2).png" width="195">
    <img src=":/TT/TTi(3).png" width="150">
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

<div class="formula">
<img src=":/TT/TTi(4).png" width="150">
</div>

<p>
где:
</p>

<ul>
<li>Tk — температура в текущем канальном интервале</li>
<li>n — количество измерений температуры в промежуточном слое</li>
</ul>

)";

    html += QString(
                "<p>Значение <b>Tn</b> в зоне <span class='value'>%1 м</span> равно "
                "<span class='value'>%2</span></p>")
                .arg(currentZone.height)
                .arg(currentZone.Tn);

    html += R"(

<p class="block">
Полученная температура относится к высоте середины слоя, обозначенной Hi.
</p>

<p>
Далее производится коррекция температуры на радиационную поправку:
</p>

<div class="formula">
<img src=":/TT/TTi(5).png" width="150">
</div>

<p>
где ΔTp определяется методом линейной интерполяции
по двухвходовой таблице радиационных поправок.
</p>

)";

    html += R"(

<p class="block">
Если U<sub>0</sub> было введено, виртуальная поправка вычисляется по формуле:
</p>

<div class="formula">
<img src=":/TT/TTi(6).png" width="150">
</div>

)";

    html += QString(
                "<p>Приземные измерения: "
                "U0 = <span class='value'>%1</span>, "
                "T0 = <span class='value'>%2</span>, "
                "P0 = <span class='value'>%3</span></p>")
                .arg(constants.U0)
                .arg(constants.T0)
                .arg(constants.P0);

    html += QString(
                "<p>Виртуальная поправка в зоне "
                "<span class='value'>%1</span> равна "
                "<span class='value'>%2</span>, "
                "высота середины слоя Hi = "
                "<span class='value'>%3</span></p>")
                .arg(currentZone.height)
                .arg(currentZone.dTvir)
                .arg(currentZone.Hi);

    html += R"(

<p class="block">
Температура на высоте середины зоны:
</p>

<div class="formula">
<img src=":/TT/TTi(7).png">
</div>

)";

    html += QString(
                "<p>Температура с учетом виртуальной поправки "
                "на высоте <span class='value'>%1</span> равна "
                "<span class='value'>%2</span></p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Tvrn);

    html += R"(

<p class="block">
Отклонение температуры от табличной в слое
ΔH = Hm − Hm-1 определяется по формуле:
</p>

<div class="formula">
<img src=":/TT/TTi(8).png">
</div>

)";

    html += QString(
                "<p>Табличная температура на высоте "
                "<span class='value'>%1</span> равна "
                "<span class='value'>%2</span>.<br>"
                "Отклонение температуры от табличной равно "
                "<span class='value'>%3</span>.</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Ttab)
                .arg(currentZone.TTi);

    html += "</div>";

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
Среднее отклонение температуры в слое дельтаH = Hm - Hm-1 в слое (0, Hm) определяется по формуле:
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

//линейная интерполяция температуры для mtd
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
/*
QString DisplayManager::HtmlTTcpm(const CellInfo& cell,
                                  const std::vector<Zone>& zones,
                                  const UserConstants& constants){

}
*/

QString DisplayManager::HtmlPPi(const CellInfo& cell, const std::vector<Zone>& zones){
    QString html;

    html += generateHeader(cell);
    html += R"(
<p>
Плотностные характеристики атмосферы определяются для тех же стандартных слоев, что и температурные, до высоты 10 км.
</p>

<p> Вычисление плотности в слое (Hm-1, Hm) производится следующим образом: определяется давление на середине стандартного слоя по формуле:</p>
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

    html += QString("<p>Плотность воздуха Пi на высоте %1 равна %2</p>")
                .arg(currentZone.Hi)
                .arg(currentZone.Pi);

    html += R"(
<p>
Расчет действительного отклонения плотности ППi и среднего отклонения плотности ППcpm
            от табличного значения производится в середине слоя по формуле:
</p>
)";

    return html;

}
