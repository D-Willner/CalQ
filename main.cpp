
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
	SQLDatabase sql_db("mydb");
    Settings s = Settings::load();

    QApplication app(argc, argv);

    sql_db.connect();
    sql_db.initialize_db();

    CalQ window(db, sql_db, s);
    window.show();
    
    QFile log("log.txt");
    bool file_ok = log.open(QFile::WriteOnly);


    //sql_db.add(db.get_foodtype("Apple"));

    //FoodType ft = sql_db.get("Apple");

    //int written = log.write(qs.toUtf8());
    log.close();

    return app.exec();
}

/* TODO:    README
            add InitializeDialog
            SQL database
            add ai photo recognition(pointless since too bad still? maybe learn how to train ai for this?)
            add ai configure dialog and more options
            improve visuals (style sheets and frames)
*/