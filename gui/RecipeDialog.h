#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qdialog.h"
#include "database/DataBase.h"
#include "database/Recipe.h"
#include "FoodTable.h"
#include <QLineEdit>
#include <QPushButton>
#include "database/DataBaseSearcher.h"
#include "SearchField.h"

class RecipeDialog :
    public QDialog
{
    Q_OBJECT

private:
    DataBase& database;
    DataBaseSearcher db_searcher;

    Recipe rec;

    QLineEdit* name_line;
    SearchField* search_field;

    FoodTable* food_table;

    QPushButton* accept_btn;
    QPushButton* toss_btn;

public:

    explicit RecipeDialog(const Recipe& r, DataBase& db, QWidget* parent = nullptr, 
        Qt::WindowFlags f = Qt::Dialog);

public slots:
    void handle_accept();

signals:
    void found(const Recipe&);
};

