
#define QT_NO_DEPRECATED_WARNINGS
#include "gui/CalQ.h"
#include "database/DataBase.h"
#include "database/Settings.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    DataBase db;
    Settings s = Settings::load();

    QApplication app(argc, argv);
    CalQ window(db,s);
    window.show();
    return app.exec();
}

/* TODO:    add InittializeDialog
            add tool tips
            SQL database
            local AI query for food information
*/