#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include "FoodTable.h"
#include "ExerciseTable.h"
#include "database/DataBase.h"

class DataBaseTab :
    public QWidget
{
    Q_OBJECT

private:
    DataBase& database;

    FoodTable* food_type_table;
    FoodTable* recipe_table;
    FoodTable* meal_table;
    ExerciseTable* exercise_table;

    void load_tables();
    void load_food_types();
    void load_recipes();
    void load_meals();
    void load_exercises();

    void clear_tables();
public:
    explicit DataBaseTab(DataBase& db, QWidget* parent = nullptr);

private slots:
    void handle_ft_change(int row, int col);
    void handle_rec_change(int row, int col);

public slots:
    void update_tables();
};

