
#define QT_NO_DEPRECATED_WARNINGS
#include "gui/CalQ.h"
#include "database/DataBase.h"
#include "database/Settings.h"
#include "network/client.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include "network/SQLDatabase.h"

int main(int argc, char *argv[])
{
    DataBase db;
    Settings s = Settings::load();
	SQLDatabase sql_db(s.get_sql_settings());

    QApplication app(argc, argv);

    //sql_db.connect();
    //sql_db.initialize_db();

    CalQ window(db, sql_db, s);
    window.show();

    return app.exec();
}

/* TODO:    README
            add ai photo recognition(pointless since too bad still? maybe learn how to train ai/opencv for this?)
            improve SettingsTab
*/