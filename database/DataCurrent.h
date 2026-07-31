#pragma once
#include <QObject>
#include <vector>
#include "Food.h"
#include "Exercise.h"
#include "WeightEntry.h"

class DataBase;

class DataCurrent : public QObject
{
	Q_OBJECT
private:
	DataBase& database;

	std::vector<Food> eaten;
	std::vector<Exercise> exercised;
	BODYWEIGHT_T weight_today;

	template<typename T>
	void add_v(const T& t, std::vector<T> v);

	template<typename T>
	bool remove_v(std::string name, std::vector<T> v);

public:

	explicit DataCurrent(DataBase& db);	//	loads food, exercise and weight of today from database

	CAL_T calories();
	WEIGHT_T protein();
	WEIGHT_T carbs();
	WEIGHT_T fats();

	BODYWEIGHT_T weight();
	void set_weight(BODYWEIGHT_T w);

	void load();

	const std::vector<Food> eaten_today();
	const std::vector<Exercise> exercised_today();

	void add_Food(const Food& f);
	bool remove_Food(std::string name);

	void add_Exercise(const Exercise& e);
	bool remove_Exercise(std::string name);
};

