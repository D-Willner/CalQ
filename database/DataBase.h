#pragma once
#include <vector>

#include "Food.h"
#include "FoodType.h"
#include "Recipe.h"
#include "Meal.h"
#include "ExerciseType.h"
#include "Exercise.h"
#include "DataCurrent.h"
#include "WeightEntry.h"

#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


class DataBase
{
private:
	std::map<std::string, FoodType> foods;

	std::map<std::string, Recipe> recipes;

	std::map<std::string, Meal> current_meals;

	std::map<std::string, ExerciseType> exercises;

	std::map<QDate, std::vector<Food>> food_history;

	std::map<QDate, std::vector<Exercise>> exercise_history;

	std::map<QDate, std::vector<WeightEntry>> weight_history;

	DataCurrent data_today;

	template<typename T>
	bool save_data(std::string file_name, const std::map<std::string, T>& data);

	template<typename T>
	bool save_history_data(std::string file_name, const std::map<QDate, std::vector<T>>& data);

	template<typename T>
	bool load_data(std::string file_name, std::map<std::string, T>& save_loc);

	template<typename T>
	bool load_history_data(std::string file_name, std::map<QDate, std::vector<T>>& save_loc);

	template<typename T>
	bool contains(const T& t, const std::map<std::string, T>& m);

	template<typename T>
	bool add(const T& t, std::map<std::string, T>& m);

	template<typename T>
	bool add(const T& t, std::map<QDate, std::vector<T>>& m, QDate date);

	//	returns true if it overwrites something with same name
	template<typename T>
	bool overwrite(const T& t, std::map<std::string, T>& m);

	//	returns true if it successfully removes something with that name
	template<typename T>
	bool remove(std::string name, std::map<std::string, T>& m);

	//	returns true if it successfully removes something with that name
	template<typename T>
	bool remove(std::string name, QDate day, std::map<QDate, std::vector<T>>& m);

	template<typename T>
	std::vector<T> Tvec_name_contains(std::string s, const std::map<std::string, T>& m);

	template<typename T>
	T get(std::string name, const std::map<std::string, T>& m);

	template<typename T>
	std::vector<std::pair<QDate, std::vector<T>>>  T_range(QDate beg, QDate end, const std::map<QDate, std::vector<T>>& m);

public:
	enum DTYPE{
		EMPTY_T,
		FOODTYPE_T,
		RECIPE_T,
		MEAL_T,
		EXERCISETYPE_T,
		EXERCISE_T,
		FOOD_T
	};

	DataBase();

	~DataBase();

	//	returns the type if data_base contains item with recipe_name
	//	otherwise return EMPTY_T
	DTYPE contains(std::string name);

	FoodType get_foodtype(std::string name);
	Recipe get_recipe(std::string name);
	Meal get_meal(std::string name);
	ExerciseType get_exercisetype(std::string name);

	std::vector<FoodType> foodtype_name_contains(std::string s);
	std::vector<Recipe> recipe_name_contains(std::string s);
	std::vector<ExerciseType> exercisetype_name_contains(std::string s);
	std::vector<Meal> meal_name_contains(std::string s);

	std::vector<Food> food_on(QDate dt = QDate::currentDate());
	std::vector<std::pair<QDate, std::vector<Food>>> food_range(QDate beg, QDate end = QDate::currentDate());
	std::vector<Exercise> exercise_on(QDate dt = QDate::currentDate());
	std::vector<std::pair<QDate, std::vector<Exercise>>> exercise_range(QDate beg, QDate end = QDate::currentDate());

	BODYWEIGHT_T last_weight();
	QDate last_weight_date();
	BODYWEIGHT_T weight_on(QDate dt = QDate::currentDate());
	std::vector<std::pair<QDate, BODYWEIGHT_T>> weights_range(QDate beg, QDate end = QDate::currentDate());

	bool overwrite(const FoodType& ft);
	bool overwrite(const ExerciseType& et);
	bool overwrite(const Recipe& r);
	bool overwrite(const Meal& m);

	bool overwrite(const WeightEntry& e, QDate day = QDate::currentDate());
	bool overwrite(BODYWEIGHT_T w, QDate day = QDate::currentDate());

	bool add(const FoodType& ft);
	bool add(const ExerciseType& et);
	bool add(const Recipe& r);
	bool add(const Meal& m);

	bool add(const Food& f, QDate = QDate::currentDate());
	bool add(const Exercise& e, QDate = QDate::currentDate());

	bool remove_foodtype(std::string name);
	bool remove_exercisetype(std::string name);
	bool remove_recipe(std::string name);
	bool remove_meal(std::string name);

	bool remove_food(std::string name, QDate day = QDate::currentDate());	//	removes one occurance of this food on the specified date
	bool remove_exercise(std::string name, QDate day = QDate::currentDate());	//	removes one occurance of this exercise on the specified date
	bool remove_weight(QDate day = QDate::currentDate());

	bool load();
	bool save();

	bool load_foods();
	bool save_foods();

	bool load_recipes();
	bool save_recipes();

	bool load_meals();
	bool save_meals();

	bool load_exercises();
	bool save_exercises();

	bool load_food_history();
	bool save_food_history();

	bool load_exercise_history();
	bool save_exercise_history();

	bool load_weight_history();
	bool save_weight_history();

	void clear();

	DataCurrent& today();
};

