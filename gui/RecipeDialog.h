#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qdialog.h"
#include "database/DataBase.h"
#include "database/Recipe.h"
#include "FoodTable.h"
#include <QLineEdit>
#include <QPushButton>

class RecipeDialog :
    public QDialog
{
    Q_OBJECT

private:
    DataBase& database;

    Recipe rec;

    QLineEdit* name_line;
    FoodTable* food_table;

    QPushButton* accept_btn;
    QPushButton* toss_btn;

public:

    explicit RecipeDialog(const Recipe& r, DataBase& db, QWidget* parent = nullptr, 
        Qt::WindowFlags f = Qt::WindowFlags());

public slots:
    void handle_accept();
};

