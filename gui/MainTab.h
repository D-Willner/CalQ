#pragma once
#define QT_NO_DEPRECATED_WARNINGS
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QTableView>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QDoubleSpinBox>

#include "database/DataBase.h"
#include "database/Settings.h"
#include "database/DataBaseSearcher.h"
#include "FoodTable.h"
#include "CalorieDisplay.h"
#include "MacroChart.h"
#include "ExerciseTable.h"
#include "SearchField.h"
#include "WeightChart.h"

class MainTab :
    public QWidget
{
    Q_OBJECT

private:
    constexpr static double MEAL_MIN_PERCENTAGE = 0.01;

    DataBase& database;
    Settings& settings;

    QHBoxLayout* layout_high;
    QVBoxLayout* layout_left;
    QVBoxLayout* layout_right;

    FoodTable* food_table;


    QHBoxLayout* layout_add_table;
    QPushButton* add_food_btn;
    QPushButton* search_btn;
    //SearchLine search_line;
    SearchField* search_eatables;
    DataBaseSearcher* eatables_searcher;
    QPushButton* add_recipe_btn;
    QLineEdit* recipe_name_line;

    FoodTable* add_table;

    CalorieDisplay* calorie_display;

    MacroChart* macro_chart;

    QDoubleSpinBox* weight_entry_editor;
    QPushButton* weight_btn;
    WeightChart* weight_chart;

    ExerciseTable* exerciseTable;
    ExerciseTable* add_exercise_table;
    QPushButton* add_exercise_btn;
    SearchField* search_exercise;
    DataBaseSearcher* exercise_searcher;

public:
    explicit MainTab(DataBase& db, Settings& s, QWidget* parent = nullptr);

    void closeEvent(QCloseEvent* event) override
    {
        database.save();

        QWidget::closeEvent(event);
    }

public:
    //void adjustRow(QTableWidgetItem* item);

private slots:
    void add_food_today();
    void remove_food_today(const Food& f);
    void add_recipe();
    void consider_food(const Food& f);
    void consider_found_eatable(std::string name);
    void add_empty_food();
    void expand_recipe(const Food&, int row);

    void add_exercise_today();
    void exercise_found(std::string name);
    void remove_exercise_today(const Exercise& ex);
    void adjust_row_ex(QTableWidgetItem* item);

    void weight_btn_clicked();
};

