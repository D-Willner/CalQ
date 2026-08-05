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
    QChart* chart;

    QBarSeries* data;

    QBarSet* protein;
    QBarSet* carbs;
    QBarSet* fats;

    QValueAxis* yaxis;

    int yaxis_is_max();
    void update_yaxis();

public:
    explicit MacroChart(QWidget* parent = nullptr);

public slots:
    WEIGHT_T get_protein();
    WEIGHT_T get_carbs();
    WEIGHT_T get_fats();

    void set_protein(WEIGHT_T p);
    void set_carbs(WEIGHT_T c);
    void set_fats(WEIGHT_T f);

    void add_protein(WEIGHT_T p);
    void add_carbs(WEIGHT_T c);
    void add_fats(WEIGHT_T f);

    void add_food(const Food& f);
    void remove_food(const Food& f);
};

