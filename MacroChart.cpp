#include "MacroChart.h"
#include <QSizePolicy>
#include <QWidget>

MacroChart::MacroChart(QWidget* parent) : QChartView(parent)
{
    m_chart = new QChart();
    //m_chart->setMinimumSize(400, 300);
    //m_chart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_data = new QBarSeries;

    m_protein = new QBarSet("Protein");
    m_carbs = new QBarSet("Carbs");
    m_fats = new QBarSet("Fats");

    m_protein->append(0);
    m_carbs->append(0);
    m_fats->append(0);

    m_data->append(m_protein);
    m_data->append(m_carbs);
    m_data->append(m_fats);

    m_chart->addSeries(m_data);

    m_yAxis = new QValueAxis;
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    m_data->attachAxis(m_yAxis);

    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(400, 300);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setChart(m_chart);
}

int MacroChart::yAxisMax()
{
    WEIGHT_T m = std::max(protein(), carbs());
    m = std::max(m, fats());

    return ((int)m / 40 + 1) * 40;
}

void MacroChart::updateYAxis()
{
    m_yAxis->setRange(0, yAxisMax());
}

WEIGHT_T MacroChart::protein() { return m_protein->at(0); }
WEIGHT_T MacroChart::carbs() { return m_carbs->at(0); }
WEIGHT_T MacroChart::fats() { return m_fats->at(0);  }

void MacroChart::setProtein(WEIGHT_T p)
{ 
    m_protein->replace(0, p); 
    updateYAxis(); 
}

void MacroChart::setCarbs(WEIGHT_T c)
{ 
    m_carbs->replace(0, c); 
    updateYAxis(); 
}

void MacroChart::setFats(WEIGHT_T f)
{ 
    m_fats->replace(0, f);
    updateYAxis(); 
}

void MacroChart::addProtein(WEIGHT_T p)
{ 
    m_protein->replace(0, m_protein->at(0) + p); 
    updateYAxis();
}

void MacroChart::addCarbs(WEIGHT_T c)
{ 
    m_carbs->replace(0, m_carbs->at(0) + c);
    updateYAxis();
}

void MacroChart::addFats(WEIGHT_T f)
{ 
    m_fats->replace(0, m_fats->at(0) + f);
    updateYAxis();
}

void MacroChart::addFood(const Food& f) 
{
    addProtein(f.protein());
    addCarbs(f.carbs());
    addFats(f.fats());
}

void MacroChart::removeFood(const Food& f)
{
    addProtein(-f.protein());
    addCarbs(-f.carbs());
    addFats(-f.fats());
}