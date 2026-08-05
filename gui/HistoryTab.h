#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>

#include "database/DataBase.h"
#include "database/Settings.h"

class HistoryTab :
    public QWidget
{
    Q_OBJECT

private:
    constexpr static std::array CHART_DATA_TYPES
        = { "None", "Weight", "Calories eaten", "Calorie delta", "Exercised calories" };

    DataBase& database;
    Settings& settings;

    QDateEdit* start_date_selector;
    QDateEdit* end_date_selector;

    QComboBox* data_one_selector;
    QComboBox* data_two_selector;

    QChartView* chart_view;
    QChart* chart;

    QDateTimeAxis* x_axis;
    QValueAxis* y1_axis;
    QValueAxis* y2_axis;

    QLineSeries* series_one;
    QLineSeries* series_two;

    void update_x_axis();
    QLineSeries* update_y_axis(QValueAxis* y_axis, std::string selection);
    void update_y1_axis();
    void update_y2_axis();

    //!!!  must be deleted or have ownership transferred
    QLineSeries* fetch_weight(QDate start, QDate end);
    QLineSeries* fetch_cal_eaten(QDate start, QDate end);
    QLineSeries* fetch_cal_diff(QDate start, QDate end);
    QLineSeries* fetch_cal_ex(QDate start, QDate end);

public:
    explicit HistoryTab(DataBase& db, Settings& s, QWidget* parent = nullptr);
};

