#include "DataBaseSearcher.h"
DataBaseSearcher::DataBaseSearcher(DataBase& db, QObject* parent) : QObject(parent), database(db) {}


QStringList DataBaseSearcher::search_foodtypes(std::string name)
{
	auto vec_ft = database.foodtype_name_contains(name);
	QStringList qs_list;
	for (const auto& ft : vec_ft) {
		qs_list.append(QString::fromStdString(ft.name()));
	}
	return qs_list;
}

QStringList DataBaseSearcher::search_eatables(std::string name)
{
	auto vec_meal = database.meal_name_contains(name);
	auto vec_rec = database.recipe_name_contains(name);
	auto vec_ft = database.foodtype_name_contains(name);

	QStringList qs_list;
	for (const auto& m : vec_meal) {
		qs_list.append(QString::fromStdString(m.name()));
	}
	for (const auto& r : vec_rec) {
		qs_list.append(QString::fromStdString(r.name()));
	}
	for (const auto& ft : vec_ft) {
		qs_list.append(QString::fromStdString(ft.name()));
	}

	return qs_list;
}

QStringList DataBaseSearcher::search_exercises(std::string name)
{
	auto vec_ex = database.exercisetype_name_contains(name);

	QStringList qs_list;
	for (const auto& ext : vec_ex) {
		qs_list.append(QString::fromStdString(ext.name()));
	}

	return qs_list;
}

void DataBaseSearcher::emit_foodtypes(std::string name) { emit results(search_foodtypes(name)); }
void DataBaseSearcher::emit_eatables(std::string name) { emit results(search_eatables(name)); }
void DataBaseSearcher::emit_exercises(std::string name) { emit results(search_exercises(name)); }