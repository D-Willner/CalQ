#pragma once
#define QT_NO_DEPRECATED_WARNINGS
#include <QtWidgets/QMainWindow>
#include <QTabWidget>
#include <QPushButton>

#include "MainTab.h"
#include "database/DataBase.h"
#include "DataBaseTab.h"
#include "HistoryTab.h"
#include "SettingsTab.h"
#include "network/SQLDatabase.h"

class CalQ : public QWidget
{
    Q_OBJECT

private:
    constexpr static int DATABASETAB_IND = 1;

    MainTab* main_tab;
    DataBaseTab* database_tab;
    HistoryTab* history_tab;
    SettingsTab* settings_tab;

    QTabWidget* tabs;

    QVBoxLayout* layout;

    DataBase& database;
	SQLDatabase sql_database;
    Settings& settings;

public:
    CalQ(DataBase& db, SQLDatabase& sql_db, Settings& s, QWidget *parent = nullptr);
    ~CalQ();

};

