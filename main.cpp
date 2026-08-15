
#define QT_NO_DEPRECATED_WARNINGS
#include "gui/CalQ.h"
#include "database/DataBase.h"
#include "database/Settings.h"
#include "network/client.h"
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

    //Client*  c = new Client;
    //c->request_food_data("Chicken");

    return app.exec();
}

/* TODO:    README
            add InitializeDialog
            SQL database
            add ai photo recognition
            add ai configure dialog and more options
            improve visuals (style sheets and frames)
*/