#include "MacroChart.h"
#include <QSizePolicy>
#include <QWidget>

MacroChart::MacroChart(QWidget* parent) : QChartView(parent)
{
    chart = new QChart();
    chart->setContentsMargins(-10, -30, -10, -30);
    //chart->setViewportMargins(0, 0, 0, 0);

    data = new QBarSeries;

    protein = new QBarSet("Protein (g)");
    carbs = new QBarSet("Carbohydrates (g)");
    fats = new QBarSet("Fats (g)");

    protein->append(0);
    carbs->append(0);
    fats->append(0);

    data->append(protein);
    data->append(carbs);
    data->append(fats);

    chart->addSeries(data);

    yaxis = new QValueAxis;
    chart->addAxis(yaxis, Qt::AlignLeft);
    data->attachAxis(yaxis);

    chart->setAnimationOptions(QChart::SeriesAnimations);

    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(400, 200);
    //setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setChart(chart);
    //setContentsMargins(0, -30, 0, 0);
    //setViewportMargins(0, -30, 0, -30);
}

int MacroChart::yaxis_is_max()
{
    WEIGHT_T m = std::max(get_protein(), get_carbs());
    m = std::max(m, get_fats());

    return ((int)m / 40 + 1) * 40;
}

void MacroChart::update_yaxis()
{
    yaxis->setRange(0, yaxis_is_max());
}

WEIGHT_T MacroChart::get_protein() { return protein->at(0); }
WEIGHT_T MacroChart::get_carbs() { return carbs->at(0); }
WEIGHT_T MacroChart::get_fats() { return fats->at(0);  }

void MacroChart::set_protein(WEIGHT_T p)
{ 
    protein->replace(0, p); 
    update_yaxis(); 
}

void MacroChart::set_carbs(WEIGHT_T c)
{ 
    carbs->replace(0, c); 
    update_yaxis(); 
}

void MacroChart::set_fats(WEIGHT_T f)
{ 
    fats->replace(0, f);
    update_yaxis(); 
}

void MacroChart::add_protein(WEIGHT_T p)
{ 
    protein->replace(0, protein->at(0) + p); 
    update_yaxis();
}

void MacroChart::add_carbs(WEIGHT_T c)
{ 
    carbs->replace(0, carbs->at(0) + c);
    update_yaxis();
}

void MacroChart::add_fats(WEIGHT_T f)
{ 
    fats->replace(0, fats->at(0) + f);
    update_yaxis();
}

void MacroChart::add_food(const Food& f) 
{
    add_protein(f.protein());
    add_carbs(f.carbs());
    add_fats(f.fats());
}

void MacroChart::remove_food(const Food& f)
{
    add_protein(-f.protein());
    add_carbs(-f.carbs());
    add_fats(-f.fats());
}