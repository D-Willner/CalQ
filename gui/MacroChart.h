#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtCharts\qchartview.h"
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include "database/Food.h"

class MacroChart :
    public QChartView
{
    Q_OBJECT

private:
    QChart* m_chart;

    QBarSeries* m_data;

    QBarSet* m_protein;
    QBarSet* m_carbs;
    QBarSet* m_fats;

    QValueAxis* m_yAxis;

    int yAxisMax();
    void updateYAxis();

public:
    explicit MacroChart(QWidget* parent = nullptr);

public slots:
    WEIGHT_T protein();
    WEIGHT_T carbs();
    WEIGHT_T fats();

    void setProtein(WEIGHT_T p);
    void setCarbs(WEIGHT_T c);
    void setFats(WEIGHT_T f);

    void addProtein(WEIGHT_T p);
    void addCarbs(WEIGHT_T c);
    void addFats(WEIGHT_T f);

    void addFood(const Food& f);
    void removeFood(const Food& f);
};

