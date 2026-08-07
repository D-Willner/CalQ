#include "MainTab.h"
#include <QValueAxis>
#include <QHeaderView>
#include <iostream>
#include <QMessageBox>
#include <cmath>
#include "ToolTip.h"
#include "ToolTipLabel.h"


MainTab::MainTab(DataBase& db, Settings& s, QWidget* parent) 
    : QWidget(parent), database(db), settings(s), search_line(db, this)
{
    database_searcher = new DataBaseSearcher(db, this);    
    
    search_line.get_input_widget()->setToolTip("Search the database for food types, recipes and meals.\n\
Alternatively add entries directly to the table.");

    macro_chart = new MacroChart;
    macro_chart->set_protein(db.today().protein());
    macro_chart->set_carbs(db.today().carbs());
    macro_chart->set_fats(db.today().fats());
    macro_chart->setFixedSize(400, 300);
    macro_chart->setToolTip("Displays the macro nutrients consumed today.");

    calorie_display = new CalorieDisplay;
    calorie_display->set_target_calories(settings.get_calorie_target());    //  should update when settings updated

    food_table = new FoodTable(5, FoodTable::NO_FACTOR);
    food_table->set_editable(false);

    add_food_btn = new QPushButton("Add food");
    add_food_btn->setToolTip("Adds all the food in the table below to foods eaten today.\nFood not in the database will automatically get added to it.");
    add_recipe_btn = new QPushButton("Add recipe");
    add_recipe_btn->setToolTip("Creates a new recipe using the ingredients in the table below with name specified to the right.\n\
It gets added to the foods eaten today and to the database, where it overwrites previous entries.\n\
Use factor to eat partially and save the rest as a meal.");
    recipe_name_line = new QLineEdit;
    recipe_name_line->setMinimumWidth(150);   
    recipe_name_line->setToolTip("Create a new recipe using the ingredients in the table below with name specified here.");

    search_btn = new QPushButton("->");
    search_btn->setFixedWidth(30);

    add_table = new FoodTable(5, FoodTable::FACTOR);
    add_table->set_editable(true);
    add_table->setToolTip("Use the search bar or directly edit the entries here.\n\
Double clicking recipes will yield their ingredients.\n\
Remove entries using the \"-\" Button.");
    
    exerciseTable = new ExerciseTable(5);
    exerciseTable->set_minimum_rows(5);

    add_exercise_table = new ExerciseTable(1);
    add_exercise_table->set_editable(true);
    //add_exercise_table->setMinimumRows(1);

    add_exercise_btn = new QPushButton("Add");
    add_exercise_btn->setToolTip("Add the exercise in the table below to the exercises done today. An exercise not in the database will automatically get added to it.");

    search_exercise = new SearchField(this);
    search_exercise->get_input_widget()->setToolTip("Search the database for exercise types already added.\nAlternatively add entries directly to the table.");

    weight_entry_editor = new QDoubleSpinBox;
    weight_entry_editor->setMaximum(500);
    weight_btn = new QPushButton("Update Weight");
    weight_btn->setToolTip("Adds a new weight entry for today using the value to the left.\n\
If one already exists, it is overwritten.");
    QDate day = QDate::currentDate();
    day = day.addDays(-7);
    weight_chart = new WeightChart(database.weights_range(day));
    weight_chart->setFixedSize(400, 200);

    layout_add_table_upper = new QHBoxLayout();
    layout_add_table_upper->addWidget(add_food_btn,1 ,Qt::AlignLeft);
    layout_add_table_upper->addWidget(search_line.get_input_widget(), 0, Qt::AlignRight);
    layout_add_table_upper->addWidget(search_btn, 0, Qt::AlignRight);

    layout_add_table_lower = new QHBoxLayout();
    layout_add_table_lower->addWidget(add_recipe_btn, 0, Qt::AlignLeft);
    layout_add_table_lower->addWidget(recipe_name_line, 1, Qt::AlignLeft);

    QHBoxLayout* layout_exercise = new QHBoxLayout();
    QVBoxLayout* layout_exercise_left = new QVBoxLayout();
    QVBoxLayout* layout_exercise_right = new QVBoxLayout();

    ToolTipLabel* tt = new ToolTipLabel("Display all the exercises done today.\n\
Press the \"-\" button to remove exercises.", "Exercised today:");
    layout_exercise_left->addWidget(tt, 1, Qt::AlignLeft);
    layout_exercise_left->addWidget(exerciseTable,1,Qt::AlignTop);

    QHBoxLayout* layout_add_exercise = new QHBoxLayout();

    layout_add_exercise->addWidget(add_exercise_btn, 1, Qt::AlignLeft);
    layout_add_exercise->addWidget(search_exercise->get_input_widget());

    layout_exercise_right->addSpacing(40);  //  should use gridlayout here instead
    layout_exercise_right->addLayout(layout_add_exercise);
    layout_exercise_right->addWidget(add_exercise_table, 1, Qt::AlignTop);


    layout_exercise->addLayout(layout_exercise_left);
    layout_exercise->addLayout(layout_exercise_right);

    layout_high = new QHBoxLayout();
    layout_left = new QVBoxLayout();
    layout_right = new QVBoxLayout();

    layout_left->addWidget(new ToolTipLabel("Displays all the food eaten today.\n\
Press the \"-\" button to remove food.", "Eaten today:"), 1, Qt::AlignLeft);
    layout_left->addWidget(food_table,0, Qt::AlignTop | Qt::AlignLeft);
    layout_left->addWidget(new QLabel(""));
    layout_left->addLayout(layout_add_table_upper);
    layout_left->addLayout(layout_add_table_lower);
    layout_left->addWidget(add_table, 0, Qt::AlignTop | Qt::AlignLeft);
    layout_left->addLayout(layout_exercise, 1);

    QHBoxLayout* layout_weight = new QHBoxLayout();
    layout_weight->addWidget(weight_entry_editor,0,Qt::AlignTop  | Qt::AlignLeft);
    layout_weight->addWidget(weight_btn, 1, Qt::AlignTop | Qt::AlignLeft);

    layout_right->addWidget(calorie_display, 0, Qt::AlignTop);
    layout_right->addWidget(macro_chart, 0, Qt::AlignTop | Qt::AlignLeft);
    layout_right->addLayout(layout_weight);
    layout_right->addWidget(weight_chart, 1, Qt::AlignTop);

    layout_high->addLayout(layout_left);
    layout_high->addLayout(layout_right);

    setLayout(layout_high);

    search_line.get_result_widget()->setParent(this);
    search_line.get_result_widget()->hide();

    search_exercise->get_result_widget()->setParent(this);

    QObject::connect(add_food_btn, SIGNAL(clicked()), this, SLOT(add_food_today()));
    QObject::connect(food_table, SIGNAL(food_removed(const Food&)), this, SLOT(remove_food_today(const Food&)));
    QObject::connect(&search_line, SIGNAL(found(const Food&)), add_table, SLOT(add_food(const Food&)));
    QObject::connect(&search_line, &SearchLine::found_meal, add_table, &FoodTable::add_meal);
    QObject::connect(&search_line, &SearchLine::found, this, &MainTab::consider_food);
    QObject::connect(add_table, &FoodTable::itemChanged, this, &MainTab::adjustRow);
    QObject::connect(add_recipe_btn, &QPushButton::clicked, this, &MainTab::add_recipe);
    QObject::connect(search_btn, &QPushButton::clicked, this, &MainTab::add_empty_food);
    QObject::connect(add_table, &FoodTable::food_double_clicked, this, &MainTab::expand_recipe);

    QObject::connect(search_exercise, &SearchField::request, database_searcher, &DataBaseSearcher::emit_exercises);
    QObject::connect(database_searcher, &DataBaseSearcher::results, search_exercise, &SearchField::update_search_results);
    QObject::connect(add_exercise_btn, &QPushButton::clicked, this, &MainTab::add_exercise_today);    
    QObject::connect(exerciseTable, &ExerciseTable::exercise_removed, this, &MainTab::remove_exercise_today);
    QObject::connect(add_exercise_table, &ExerciseTable::itemChanged, this, &MainTab::adjust_row_ex);
    QObject::connect(search_exercise, &SearchField::found, this, &MainTab::exercise_found);

    QObject::connect(weight_btn, &QPushButton::clicked, this, &MainTab::weight_btn_clicked);

    setFocusPolicy(Qt::ClickFocus);

    for (const auto& f : database.today().eaten_today()) {
        food_table->add_food(f);
        calorie_display->add_consumed_calories(f.calories());
    }

    for (const auto& e : database.today().exercised_today()) {
        exerciseTable->addExercise(e);
        calorie_display->add_exercised_calories(e.calories());
    }

    BODYWEIGHT_T w = database.last_weight();
    if (w != -1) weight_entry_editor->setValue(w);
}

void MainTab::weight_btn_clicked()
{
    BODYWEIGHT_T w = weight_entry_editor->value();
    if (w < 0) return;
    database.today().set_weight(w);
    weight_chart->add(w);
}

void MainTab::exercise_found(std::string name)
{
    if (database.contains(name) != DataBase::DTYPE::EXERCISETYPE_T) return;

    ExerciseType et = database.get_exercisetype(name);

    add_exercise_table->addExercise(Exercise(et, 60));
}

void MainTab::add_exercise_today()
{
    if (add_exercise_table->rowCount() < 1 || !add_exercise_table->has_exercise(0)) return;

    Exercise ex = add_exercise_table->read_exercise(0);
    auto type = database.contains(ex.name());

    exerciseTable->addExercise(ex);
    database.today().add_exercise(ex);
    database.add(ex.exercise_type());
    calorie_display->add_exercised_calories(ex.calories());

    add_exercise_table->clear_table();
}

void MainTab::remove_exercise_today(const Exercise& ex)
{
    database.today().remove_exercise(ex.name());
    calorie_display->add_exercised_calories(-ex.calories());
}

void MainTab::adjust_row_ex(QTableWidgetItem* item)
{
    int r = item->row();
    if (r != 0 || item->column() != 1) return;

    if (!add_exercise_table->has_exercise(r)) return;

    Exercise ex = add_exercise_table->read_exercise(r);
    auto type = database.contains(ex.name());
    if (type != DataBase::DTYPE::EXERCISETYPE_T) return;

    ExerciseType et = database.get_exercisetype(ex.name());
    add_exercise_table->set_exercise_noduration(Exercise(et, ex.duration()), r);
}

void MainTab::add_recipe()
{
    std::string name = recipe_name_line->text().toStdString();
    if (name == "") return;

    std::vector<Food> foods;

    for (int i = 0; i < add_table->rowCount(); i++) {
        if (add_table->has_food(i)) foods.push_back(add_table->read_food(i));
    }

    recipe_name_line->setText("");

    Recipe r(name, foods);
    database.add(r);

    add_table->clear_table();
    add_table->add_food(r.as_food());
    add_food_today();
}

void MainTab::add_empty_food()
{
    std::string name = search_line.text();
    if (database.contains(name) == DataBase::DTYPE::EMPTY_T) {
        add_table->add_food(Food(FoodType(name), 0));
        search_line.clear_text();
    }
    else {
        search_line.clicked_name(name);
    }
}

void MainTab::consider_food(const Food& f)
{
    auto type = database.contains(f.name());

    if (type == DataBase::DTYPE::RECIPE_T) {
        recipe_name_line->setText(QString::fromStdString(f.name()));
    }
}

void MainTab::add_food_today()
{
    for (int i = 0; i < add_table->rowCount(); i++) {
        if (!add_table->has_food(i)) continue;

        Food f = add_table->read_food(i);
        auto type = database.contains(f.name());
        if(type == DataBase::DTYPE::MEAL_T) {
            Meal m = database.get_meal(f.name());
            double rem = m.percentage_left();
            
            double factor = rem;
            if (add_table->has_factor(i)) {
                factor = add_table->read_factor(i);
            }
            f *= factor;

            if (rem - factor > MEAL_MIN_PERCENTAGE) {
                m.set_percentage_left(rem - factor);
                database.overwrite(m);
            }
            else {
                database.remove_meal(m.name());
            }
        } else if (add_table->has_factor(i)) {
            double factor = add_table->read_factor(i);
            Food food_without_factor = f;
            f *= factor;

            double int_part;
            double double_part = std::modf(factor, &int_part);
            if (double_part > MEAL_MIN_PERCENTAGE && (f.name() != "")) {
                double rem = 1 - double_part;
                Meal m(food_without_factor, rem, 1);
                database.add(m);
            }
        }

        if (f.name() != "" || f.calories() != 0) {
            database.today().add_food(f);
            food_table->add_food(f);

            calorie_display->add_consumed_calories(f.calories());
            macro_chart->add_food(f);
        }

        if (f.name() != "" && type != DataBase::DTYPE::MEAL_T && type != DataBase::DTYPE::RECIPE_T) {
            database.add(f.food_type());
        }

    }

    add_table->clear_table();
    recipe_name_line->clear();
}

void MainTab::remove_food_today(const Food& f)
{
    database.today().remove_food(f.name());
    calorie_display->add_consumed_calories(-f.calories());
    macro_chart->remove_food(f);
}

void MainTab::adjustRow(QTableWidgetItem* item)
{
    if (item->column() == 1 && add_table->item(item->row(), 0) != nullptr) {
        int row = item->row();
        std::string name = add_table->item(row, 0)->text().toStdString();
        auto type = database.contains(name);
        if (type == DataBase::FOODTYPE_T) {
            FoodType ft = database.get_foodtype(name);
            bool conv;
            int weight = item->text().toInt(&conv);
            if (conv) {
                Food f(ft, weight);
                add_table->set_food_no_amount(f, row);
            }
        }
    }
}

void MainTab::expand_recipe(const Food& f, int row)
{
    if (database.contains(f.name()) != DataBase::DTYPE::RECIPE_T) return;

    Recipe r = database.get_recipe(f.name());
    recipe_name_line->setText(QString::fromStdString(r.name()));
    add_table->clear_remove_emit_row(0);
    for (auto& f : r.get_ingredients()) {
        add_table->insert_food(f, 0);
    }
}