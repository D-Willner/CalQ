#pragma once
#define QT_NO_DEPRECATED_WARNINGS
#include <QtWidgets/QMainWindow>
#include <QTabWidget>
#include <QPushButton>

#include "MainTab.h"
#include "DataBase.h"
#include "DataBaseTab.h"
#include "HistoryTab.h"
#include "SettingsTab.h"

class CalQ : public QWidget
{
    Q_OBJECT

private:
    MainTab* main_tab;
    DataBaseTab* database_tab;
    HistoryTab* history_tab;
    SettingsTab* settings_tab;

    QTabWidget* tabs;

    QVBoxLayout* layout;

    DataBase& database;

public:
    CalQ(DataBase& db, QWidget *parent = nullptr);
    ~CalQ();

};

