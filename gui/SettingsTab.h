#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "database/Settings.h"

class SettingsTab :
    public QWidget
{
    Q_OBJECT
private:
    Settings& settings;

    QDoubleSpinBox* target_weight_sb;
    QDoubleSpinBox* starting_weight_sb;

    QSpinBox* calorie_target_sb;
    QSpinBox* cal_burn_daily_sb;

    QPushButton* initialize_btn;

    QPushButton* accept_btn;
    QPushButton* deny_btn;

    void set();
    void fetch();

public:
    explicit SettingsTab(Settings& s, QWidget* parent = nullptr);
};

