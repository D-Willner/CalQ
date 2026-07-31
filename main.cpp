
#define QT_NO_DEPRECATED_WARNINGS
#include "gui/CalQ.h"
#include "database/DataBase.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    DataBase db;
    /*
    FoodType ft_noodle("Noodle", 350, 10, 70, 5, 120);
    FoodType ft_rice("Rice", 300, 20, 65, 3, 80);
    FoodType ft_potato("Potato", 150, 5, 30, 2, 80);
    //std::vector<FoodType> v = { ft_noodle, ft_rice, ft_potato };
    db.add(ft_noodle);
    db.add(ft_rice);
    db.add(ft_potato);
    //bool suc = db.load_foods();
    db.save_foods();
    */

    QApplication app(argc, argv);
    CalQ window(db);
    window.show();
    return app.exec();
}

/* TODO:    add the other tabs
            fix rounding stuff(fixed??? kind of???)
            add weight tracking
*/