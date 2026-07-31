#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtCharts\qchartview.h"
#include <QLineSeries>
#include <qdatetimeaxis.h>
#include <QValueAxis>

#include "database/DataBase.h"


class WeightChart :
    public QChartView
{
    Q_OBJECT

private:
    QChart* chart;
    QLineSeries* data;

    QValueAxis* y_axis;
    QDateTimeAxis* x_axis;

    void update_ticks();
    void update_date_range();
    void update_value_range();

    void update_axes();

public:
    //  vector such that each QDate only appears at most once
    explicit WeightChart(const std::vector<std::pair<QDate,BODYWEIGHT_T>>& v, QWidget* parent = nullptr);
    explicit WeightChart(QWidget* parent = nullptr);

public slots:
    void add(const std::vector<std::pair<QDate, BODYWEIGHT_T>>& v);
    void add(BODYWEIGHT_T w, QDate date = QDate::currentDate());
    void add(std::pair<QDate, BODYWEIGHT_T> p);
    void remove(QDate date);
};

