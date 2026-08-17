
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
#include "network/SQL_database.h"


int main(int argc, char *argv[])
{
    DataBase db;
    Settings s = Settings::load();

    QApplication app(argc, argv);
    CalQ window(db,s);
    window.show();
    
    QFile log("log.txt");
    bool file_ok = log.open(QFile::WriteOnly);

    SQL_database sql_database("mydb");
    sql_database.connect();
    sql_database.initialize_db();

    sql_database.add(db.get_foodtype("Apple"));

    FoodType ft = sql_database.get("Apple");

    //int written = log.write(qs.toUtf8());
    log.close();

    return app.exec();
}

/* TODO:    README
            add InitializeDialog
            SQL database
            add ai photo recognition(pointless since too bad still?)
            add ai configure dialog and more options
            improve visuals (style sheets and frames)
*/