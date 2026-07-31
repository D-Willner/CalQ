#include "DataBaseTab.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "RecipeDialog.h"

DataBaseTab::DataBaseTab(DataBase& db, QWidget* parent) : QWidget(parent), database(db)
{
	food_type_table = new FoodTable(5);
	food_type_table->set_name_editable(false);
	recipe_table = new FoodTable(5);
	//recipe_table->set_name_editable(false);
	recipe_table->set_editable(false);
	meal_table = new FoodTable(5);
	//meal_table->set_name_editable(false);
	meal_table->set_editable(false);

	exercise_table = new ExerciseTable(5);
	exercise_table->set_name_editable(false);

	QVBoxLayout* layout_left = new QVBoxLayout;

	layout_left->addWidget(new QLabel("Foods:"), 0, Qt::AlignLeft);
	layout_left->addWidget(food_type_table, 0, Qt::AlignLeft);
	layout_left->addWidget(new QLabel("Recipes:"), 0, Qt::AlignLeft);
	layout_left->addWidget(recipe_table, 0, Qt::AlignLeft);
	layout_left->addWidget(new QLabel("Meals:"), 0, Qt::AlignLeft);
	layout_left->addWidget(meal_table, 0, Qt::AlignLeft);

	QVBoxLayout* layout_right = new QVBoxLayout;

	layout_right->addWidget(new QLabel("Exercises:"), 0, Qt::AlignLeft | Qt::AlignTop);
	layout_right->addWidget(exercise_table,1, Qt::AlignRight | Qt::AlignTop);

	QHBoxLayout* layout = new QHBoxLayout;

	layout->addLayout(layout_left,1);
	layout->addLayout(layout_right);

	setLayout(layout);

	//load_tables();
	QObject::connect(food_type_table, &FoodTable::food_removed,
		this, [=](const Food& f) {database.remove_foodtype(f.name());});
	QObject::connect(recipe_table, &FoodTable::food_removed,
		this, [=](const Food& f) {database.remove_recipe(f.name());});
	QObject::connect(meal_table, &FoodTable::food_removed,
		this, [=](const Food& f) {database.remove_meal(f.name());});
	QObject::connect(exercise_table, &ExerciseTable::exerciseRemoved,
		this, [=](const Exercise& e) {database.remove_exercisetype(e.name());});

	QObject::connect(food_type_table, &FoodTable::cellChanged, this, &DataBaseTab::handle_ft_change);
	QObject::connect(recipe_table, &FoodTable::cellDoubleClicked, this, &DataBaseTab::handle_rec_change);

	setFocusPolicy(Qt::ClickFocus);
}

void DataBaseTab::handle_ft_change(int row, int col)
{
	if (row >= food_type_table->rowCount() || col >= 6) return;

	if(col > 0 && food_type_table->has_food(row)){
		Food f = food_type_table->read_food(row);
		if(f.name() != "") database.overwrite(f.food_type());
	}
}

void DataBaseTab::handle_rec_change(int row, int col)
{
	if (row >= recipe_table->rowCount() || col >= 6) return;

	auto name_item = recipe_table->item(row, 0);
	if (!name_item) return;

	std::string name = name_item->text().toStdString();
	auto res = database.contains(name);
	if (res != DataBase::RECIPE_T) return;
	
	Recipe r = database.get_recipe(name);
	RecipeDialog* rd = new RecipeDialog(r,database,this);
	rd->show();
}


void DataBaseTab::load_tables()
{
	load_food_types();
	load_recipes();
	load_meals();
	load_exercises();
}

void DataBaseTab::load_food_types()
{
	std::vector<FoodType> vft = database.foodtype_name_contains("");
	for (const auto& ft : vft) {
		food_type_table->add_food(Food(ft,ft.get_size()));
	}
	for (int row = 0; row < food_type_table->rowCount(); row++) {
		auto name_item = food_type_table->item(row, 0);
		if(name_item != nullptr) name_item->setFlags(name_item->flags() & ~Qt::ItemIsEditable);
	}
}

void DataBaseTab::load_recipes()
{
	std::vector<Recipe> vr = database.recipe_name_contains("");
	for (const auto& r : vr) {
		recipe_table->add_food(r.as_food());
	}
}

void DataBaseTab::load_meals()
{
	std::vector<Meal> vm = database.meal_name_contains("");
	for (const auto& m : vm) {
		meal_table->add_meal(m);
	}
}

void DataBaseTab::load_exercises()
{
	std::vector<ExerciseType> ve = database.exercisetype_name_contains("");
	for (const auto& e : ve) {
		exercise_table->addExercise(Exercise(e, 60));
	}
}

void DataBaseTab::clear_tables()
{
	food_type_table->clear_table();
	recipe_table->clear_table();
	meal_table->clear_table();
	exercise_table->clear_table();
}


void DataBaseTab::update_tables()
{
	clear_tables();
	load_tables();
}