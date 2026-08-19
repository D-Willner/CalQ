#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qdialog.h"
#include "network/SQLDatabase.h"
#include "database/Settings.h"

class SQLConfigDialog :
    public QDialog
{
    Q_OBJECT

private:
    SQLDatabase& sql_database; 
	Settings& settings;

public:
	SQLConfigDialog(SQLDatabase& sql_db, Settings& s, QWidget* parent = nullptr);
};

