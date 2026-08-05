#include "DataCurrent.h"
#include "DataBase.h"

DataCurrent::DataCurrent(DataBase& db) : database(db) { };

void DataCurrent::load() 
{ 
	eaten = database.food_on(); 
	exercised = database.exercise_on();

	weight_today = database.weight_on();	//	-1 -> no weight recorded today
}

template<typename T>
void DataCurrent::add_v(const T& t, std::vector<T> v)
{
	v.push_back(t);
}

template<typename T>
bool DataCurrent::remove_v(std::string name, std::vector<T> v)
{
	bool ret = false;
	for (auto it = v.begin(); it != v.end(); it++) {
		if (it->name() == name) {
			v.erase(it);
			return true;
		}
	}

	return false;
}

BODYWEIGHT_T DataCurrent::weight() { return weight_today; }

void DataCurrent::set_weight(BODYWEIGHT_T w) 
{ 
	weight_today = w; 
	database.overwrite(w);
}


void DataCurrent::add_food(const Food& f) 
{ 
	add_v(f, eaten); 
	database.add(f); 
}

bool DataCurrent::remove_food(std::string name) 
{
	database.remove_food(name);
	return remove_v(name, eaten); 
}

void DataCurrent::add_exercise(const Exercise& e) 
{ 
	add_v(e, exercised);
	database.add(e);
}

bool DataCurrent::remove_exercise(std::string name) 
{ 
	database.remove_exercise(name);
	return remove_v(name, exercised); 
}

CAL_T DataCurrent::calories()
{
	CAL_T sum = 0;
	for (const Food& f : eaten) {
		sum += f.calories();
	}

	return sum;
}

WEIGHT_T DataCurrent::protein()
{
	WEIGHT_T sum = 0;
	for (const Food& f : eaten) {
		sum += f.protein();
	}

	return sum;
}

WEIGHT_T DataCurrent::carbs()
{
	WEIGHT_T sum = 0;
	for (const Food& f : eaten) {
		sum += f.carbs();
	}

	return sum;
}

WEIGHT_T DataCurrent::fats()
{
	WEIGHT_T sum = 0;
	for (const Food& f : eaten) {
		sum += f.fats();
	}

	return sum;
}

const std::vector<Food> DataCurrent::eaten_today() { return eaten; }

const std::vector<Exercise> DataCurrent::exercised_today() { return exercised;  }