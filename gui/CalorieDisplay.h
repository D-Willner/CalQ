#pragma once
#define QT_NO_DEPRECATED_WARNINGS
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include "CalorieBars.h"
#include "ToolTipLabel.h"

class CalorieDisplay :
    public QWidget
{
    Q_OBJECT

private:
    CAL_T calories_consumed;
    CAL_T calories_exercised;
    CAL_T calorie_target;

    QVBoxLayout* layout;
    ToolTipLabel* name_label;
    CalorieBars* draw_area;

public:
    explicit CalorieDisplay(QWidget* parent = nullptr);

    CAL_T get_target_calories() const;
    CAL_T get_consumed_calories() const;
    CAL_T get_exercised_calories() const;

public slots:

    void set_target_calories(CAL_T);
    void set_consumed_calories(CAL_T);
    void set_exercised_calories(CAL_T);

    void add_target_calories(CAL_T);
    void add_consumed_calories(CAL_T);
    void add_exercised_calories(CAL_T);
};

