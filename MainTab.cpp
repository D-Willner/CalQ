#include "MainTab.h"
#include <QValueAxis>
#include <QHeaderView>
#include <iostream>
#include <QMessageBox>
#include <cmath>


MainTab::MainTab(DataBase& db, QWidget* parent) : QWidget(parent), database(db), search_line(db, this)
{
    database_searcher = new DataBaseSearcher(db, this);

    m_macroChart = new MacroChart;
    m_macroChart->setProtein(db.today().protein());
    m_macroChart->setCarbs(db.today().carbs());
    m_macroChart->setFats(db.today().fats());
    m_macroChart->setFixedSize(400, 300);

    calorie_display = new CalorieDisplay;
    calorie_display->set_target_calories(1500); //  Make this a setting

    food_table = new FoodTable(5, FoodTable::NO_FACTOR);

    food_table->set_editable(false);
    food_table->set_min_rows(5);
    food_table->setFixedHeight(food_table->rowHeight(0) * 6);

    add_food_btn = new QPushButton("Add food");
    add_recipe_btn = new QPushButton("Add recipe");
    recipe_name_line = new QLineEdit;
    recipe_name_line->setMinimumWidth(150);

    search_btn = new QPushButton("->");
    search_btn->setFixedWidth(30);

    add_table = new FoodTable(5, FoodTable::FACTOR);
    add_table->set_editable(true);
    add_table->setMinimumHeight(add_table->rowHeight(0) * 6);
    add_table->setMaximumHeight(add_table->rowHeight(0) * 6);
    add_table->set_min_rows(5);
    
    exerciseTable = new ExerciseTable(5);
    exerciseTable->setFixedHeight(exerciseTable->rowHeight(0) * 6);
    exerciseTable->setMinimumRows(5);

    add_exercise_table = new ExerciseTable(1);
    add_exercise_table->setFixedHeight(add_exercise_table->rowHeight(0) * 2);
    add_exercise_table->setEditable(true);
    add_exercise_table->setMinimumRows(1);

    add_exercise_btn = new QPushButton("Add");

    search_exercise = new SearchField(this);

    weight_entry_editor = new QDoubleSpinBox;
    weight_entry_editor->setMaximum(500);
    weight_btn = new QPushButton("Update Weight");
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

    layout_exercise_left->addWidget(new QLabel("Exercised today:"));
    layout_exercise_left->addWidget(exerciseTable,1,Qt::AlignTop);

    QHBoxLayout* layout_add_exercise = new QHBoxLayout();

    layout_add_exercise->addWidget(add_exercise_btn, 1, Qt::AlignLeft);
    layout_add_exercise->addWidget(search_exercise->get_input_widget());

    layout_exercise_right->addWidget(new QLabel(""));
    layout_exercise_right->addLayout(layout_add_exercise);
    layout_exercise_right->addWidget(add_exercise_table, 1, Qt::AlignTop);


    layout_exercise->addLayout(layout_exercise_left);
    layout_exercise->addLayout(layout_exercise_right);

    layout_high = new QHBoxLayout();
    layout_left = new QVBoxLayout();
    layout_right = new QVBoxLayout();

    layout_left->addWidget(new QLabel("Today: "));
    layout_left->addWidget(food_table,0, Qt::AlignTop | Qt::AlignLeft);
    layout_left->addWidget(new QLabel(""));
    layout_left->addLayout(layout_add_table_upper);
    layout_left->addLayout(layout_add_table_lower);
    layout_left->addWidget(add_table, 0, Qt::AlignTop | Qt::AlignLeft);
    layout_left->addLayout(layout_exercise, 1);

    QHBoxLayout* layout_weight = new QHBoxLayout();
    layout_weight->addWidget(weight_entry_editor,0,Qt::AlignTop);
    layout_weight->addWidget(weight_btn, 0, Qt::AlignTop);

    layout_right->addWidget(calorie_display, 0, Qt::AlignTop);
    layout_right->addWidget(m_macroChart, 0, Qt::AlignTop | Qt::AlignLeft);
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
    QObject::connect(exerciseTable, &ExerciseTable::exerciseRemoved, this, &MainTab::remove_exercise_today);
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
    if (add_exercise_table->rowCount() < 1 || !add_exercise_table->hasExercise(0)) return;

    Exercise ex = add_exercise_table->readExercise(0);
    auto type = database.contains(ex.name());

    exerciseTable->addExercise(ex);
    database.today().add_Exercise(ex);
    database.add(ex.exercise_type());
    calorie_display->add_exercised_calories(ex.calories());

    add_exercise_table->clearTable();
}

void MainTab::remove_exercise_today(const Exercise& ex)
{
    database.today().remove_Exercise(ex.name());
    calorie_display->add_exercised_calories(-ex.calories());
}

void MainTab::adjust_row_ex(QTableWidgetItem* item)
{
    int r = item->row();
    if (r != 0 || item->column() != 1) return;

    if (!add_exercise_table->hasExercise(r)) return;

    Exercise ex = add_exercise_table->readExercise(r);
    auto type = database.contains(ex.name());
    if (type != DataBase::DTYPE::EXERCISETYPE_T) return;

    ExerciseType et = database.get_exercisetype(ex.name());
    add_exercise_table->setExerciseNoDuration(Exercise(et, ex.duration()), r);
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
            database.today().add_Food(f);
            food_table->add_food(f);

            calorie_display->add_consumed_calories(f.calories());
            m_macroChart->addFood(f);
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
    database.today().remove_Food(f.name());
    calorie_display->add_consumed_calories(-f.calories());
    m_macroChart->removeFood(f);
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
    add_table->clear_remove_row(0);
    for (auto& f : r.get_ingredients()) {
        add_table->insert_food(f, 0);
    }
}