#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include <QColor>
#include "database/Food.h"

class CalorieBars :
    public QWidget
{
    Q_OBJECT

private:
    CAL_T calories_consumed;
    CAL_T calories_exercised;
    CAL_T calorie_target;

    constexpr static int MIN_WIDTH = 100;
    constexpr static int MIN_HEIGHT = 60;

    constexpr static int MARGIN = 5;
    constexpr static int STANDARD_BAR_HEIGHT = 20;

    constexpr static int FONT_SIZE = 10;

    constexpr static QColor LIGHT_GREEN = QColor(6, 214, 107);
    constexpr static QColor LIGHT_BLUE = QColor(6, 193, 214);
    constexpr static QColor LIGHT_PURPLE = QColor(196, 94, 224);

public:
    explicit CalorieBars(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent*) override;

    void set_target_calories(CAL_T);
    void set_consumed_calories(CAL_T);
    void set_exercised_calories(CAL_T);
};

