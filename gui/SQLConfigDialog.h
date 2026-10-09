#pragma once
#include "qdialog.h"
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

