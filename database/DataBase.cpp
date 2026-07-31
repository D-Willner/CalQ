#include "DataBase.h"
#include <cctype>

/*
bool DataBase::load_foods()
{
	QFile loadFile("resources/foodtypes.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray data = loadFile.readAll();

    QJsonDocument doc;

    doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning("Save file wrong format.");
        return false;
    }

    QJsonObject j = doc.object();
    if (!j.contains("foods") || !j["foods"].isArray()) {
        qWarning("Save file object wrong format.");
        return false;
    }

    foods.clear();

    for (const QJsonValue& val : j["foods"].toArray()) {
        if (!val.isObject()) {
            qWarning("Save file array object wrong format.");
            return false;
        }

        FoodType ft = FoodType::from_json(val.toObject());
        foods.push_back(ft);
    }

    return true;
}

bool DataBase::save_foods()
{
    QFile saveFile("resources/foodtypes.json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonArray arr_foods;
    for (FoodType& t : foods)
        arr_foods.append(t.to_json());

    QJsonObject j;  //  argueably unnecessary
    j["foods"] = arr_foods;

    QJsonDocument doc(j);

    return saveFile.write(doc.toJson());
}

*/

DataBase::DataBase() : data_today(*this) 
{ 
    load(); 
    data_today.load();
};

DataBase::~DataBase() { save(); }

DataBase::DTYPE DataBase::contains(std::string name)
{
    if (current_meals.count(name) > 0) return MEAL_T;
    if (recipes.count(name) > 0) return RECIPE_T;
    if (foods.count(name) > 0) return FOODTYPE_T;
    if (exercises.count(name) > 0) return EXERCISETYPE_T;

    return EMPTY_T;
}

std::vector<Food> DataBase::food_on(QDate dt)
{
    if (food_history.count(dt) > 0) return food_history[dt];
    else return {};
}

std::vector<Exercise> DataBase::exercise_on(QDate dt)
{
    if (exercise_history.count(dt) > 0) return exercise_history[dt];
    else return {};
}

BODYWEIGHT_T DataBase::last_weight()
{
    if (weight_history.size() == 0) return -1;
    auto it = --weight_history.end();
    return it->second[0].get_weight();
}

QDate DataBase::last_weight_date()
{
    if (weight_history.size() == 0) throw new std::exception("No weight entries");
    auto it = --weight_history.end();
    return it->first;
}

BODYWEIGHT_T DataBase::weight_on(QDate dt)
{
    if (weight_history.count(dt) > 0 && weight_history[dt].size() > 0) return weight_history[dt][0].get_weight();
    else return -1;
}

std::vector<std::pair<QDate, BODYWEIGHT_T>> DataBase::weights_range(QDate beg, QDate end)
{
    std::vector<std::pair<QDate, BODYWEIGHT_T>> ret;
    for (QDate it = beg; it <= end; it++){
        BODYWEIGHT_T w = weight_on(it);
        if (w >= 0) ret.push_back({ it, w });
    }
    return ret;
}

template<typename T>
bool DataBase::contains(const T& t, const std::map<std::string, T>& m)
{
    return m.count(t) > 0;
}

template<typename T>
bool DataBase::add(const T& t, std::map<std::string, T>& m)
{
    if (contains(t.name()) == DTYPE::EMPTY_T) {
        m.insert({ t.name(), t });
        return true;
    }
    else {
        return false;
    }
}

bool DataBase::add(const FoodType& ft){ return add(ft, foods); }

bool DataBase::add(const ExerciseType& et) { return add(et, exercises); }

bool DataBase::add(const Recipe& r) { return add(r, recipes); }

bool DataBase::add(const Meal& m) { return add(m, current_meals); }


template<typename T>
bool DataBase::add(const T& t, std::map<QDate, std::vector<T>>& m, QDate date)
{
    m[date].push_back(t);
    return true;
}

bool DataBase::add(const Food& f, QDate date) { return add(f, food_history, date); }

bool DataBase::add(const Exercise& e, QDate date) { return add(e, exercise_history, date); }

template<typename T>
bool DataBase::save_data(std::string file_name, const std::map<std::string, T>& data)
{
    QFile saveFile(("resources/" + file_name).c_str());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonArray arr;
    for (const auto& t : data)
        arr.append(t.second.to_json());

    QJsonDocument doc(arr);

    return saveFile.write(doc.toJson());
}


template<typename T>
bool DataBase::load_data(std::string file_name, std::map<std::string, T>& save_loc)
{
    QFile loadFile(("resources/" + file_name).c_str());

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray bytes = loadFile.readAll();

    QJsonDocument jsdoc = QJsonDocument::fromJson(bytes);

    if (!jsdoc.isArray())
        return false;

    QJsonArray arr = jsdoc.array();

    for (const QJsonValue& val : arr) {
        if (!val.isObject()) return false;

        auto d = T::from_json(val.toObject());
        save_loc.insert({ d.name(), d });
    }

    return true;
}

bool DataBase::load_foods() { return load_data("foodtypes.json", foods); }

bool DataBase::save_foods() { return save_data("foodtypes.json", foods); }

bool DataBase::load_recipes() { return load_data("recipes.json", recipes); }

bool DataBase::save_recipes() { return save_data("recipes.json", recipes); }

bool DataBase::load_meals() { return load_data("meals.json", current_meals); }

bool DataBase::save_meals() { return save_data("meals.json", current_meals); }

bool DataBase::load_exercises() { return load_data("exercises.json", exercises); }

bool DataBase::save_exercises() { return save_data("exercises.json", exercises); }



template<typename T>
QJsonObject datepair_to_json(const std::pair<T, QDate>& p) {
    QJsonObject jo;
    jo["first"] = p.first.to_json();
    jo["second"] = p.second.toString();

    return jo;
}

template<typename T>
std::pair<T, QDate> datepair_from_json(const QJsonObject& jo) {
    T t = T::from_json(jo["first"].toObject());
    QDate date = QDate::fromString(jo["second"].toString());

    return { t, date };
}

template<typename T>
bool DataBase::save_history_data(std::string file_name, const std::map<QDate, std::vector<T>>& data)
{
    QFile saveFile(("resources/" + file_name).c_str());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonArray arr;

    for (const auto& p : data) {
        std::string date = p.first.toString().toStdString();
        QJsonArray date_val;
        for (const auto& t : p.second) {
            date_val.append(t.to_json());
        }

        QJsonObject date_obj;
        date_obj["date"] = date.c_str();
        date_obj["date_values"] = date_val;

        arr.append(date_obj);
    }

    QJsonDocument doc(arr);

    return saveFile.write(doc.toJson());
}

template<typename T>
bool DataBase::load_history_data(std::string file_name, std::map<QDate, std::vector<T>>& save_loc)
{
    QFile loadFile(("resources/" + file_name).c_str());

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray bytes = loadFile.readAll();

    QJsonDocument jsdoc = QJsonDocument::fromJson(bytes);

    if (!jsdoc.isArray())
        return false;

    QJsonArray arr = jsdoc.array();

    for (const QJsonValue& val : arr) {
        if (!val.isObject()) return false;
        QJsonObject history = val.toObject();

        if (!history.contains("date") || !history["date"].isString()) return false;
        QDate q_date = QDate::fromString(val["date"].toString());
        //std::string date = q_date.toStdString();

        if (!history.contains("date_values") || !history["date_values"].isArray()) return false;
        QJsonArray date_values = history["date_values"].toArray();

        for (auto val : date_values) {
            if (!val.isObject()) return false;
            save_loc[q_date].push_back(T::from_json(val.toObject()));
        }
    }

    return true;
}


bool DataBase::load_food_history() { return load_history_data("food_history.json", food_history); }

bool DataBase::save_food_history() { return save_history_data("food_history.json", food_history); }

bool DataBase::load_exercise_history() { return load_history_data("exercise_history.json", exercise_history); }

bool DataBase::save_exercise_history() { return save_history_data("exercise_history.json", exercise_history); }

bool DataBase::load_weight_history() { return load_history_data("weight_history.json", weight_history); }

bool DataBase::save_weight_history() { return save_history_data("weight_history.json", weight_history); }

bool DataBase::load()
{
    bool res = true;
    if (!load_foods()) res = false;
    if (!load_recipes()) res = false;
    if (!load_meals()) res = false;
    if (!load_exercises()) res = false;
    if (!load_food_history()) res = false;
    if (!load_exercise_history()) res = false;
    if (!load_weight_history()) res = false;

    return res;
}

bool DataBase::save()
{
    bool res = true;
    if (!save_foods()) res = false;
    if (!save_recipes()) res = false;
    if (!save_meals()) res = false;
    if (!save_exercises()) res = false;
    if (!save_food_history()) res = false;
    if (!save_exercise_history()) res = false;
    if (!save_weight_history()) res = false;

    return res;
}

void DataBase::clear()
{
    foods.clear();
    recipes.clear();
    current_meals.clear();
    exercises.clear();

    food_history.clear();
    exercise_history.clear();
    weight_history.clear();
}


template<typename T>
T DataBase::get(std::string name, const std::map<std::string, T>& m)
{
    auto it = m.find(name);
    if (it == m.end()) throw new std::exception("Could not find name in map");

    return it->second;
}

FoodType DataBase::get_foodtype(std::string name) { return get(name, foods); }

Recipe DataBase::get_recipe(std::string name) { return get(name, recipes); }

Meal DataBase::get_meal(std::string name) { return get(name, current_meals); }

ExerciseType DataBase::get_exercisetype(std::string name) { return get(name, exercises); }


template<typename T>
bool DataBase::overwrite(const T& t, std::map <std::string, T>& m)
{
    auto ret = m.insert_or_assign(t.name(), t);
    return ret.second;
}


bool DataBase::overwrite(const FoodType& ft) { return overwrite(ft, foods); }

bool DataBase::overwrite(const ExerciseType& et) { return overwrite(et, exercises); }

bool DataBase::overwrite(const Recipe& r) { return overwrite(r, recipes); }

bool DataBase::overwrite(const Meal& m) { return overwrite(m, current_meals); }

bool DataBase::overwrite(const WeightEntry& e, QDate day)
{
    return weight_history.insert_or_assign(day, std::vector<WeightEntry>({e})).second;
}

bool DataBase::overwrite(BODYWEIGHT_T w, QDate day) { return overwrite(WeightEntry(w), day); }

bool DataBase::remove_weight(QDate day)
{
    return weight_history.erase(day) > 0;
}

//	returns true if it successfully removes something with that name
template<typename T>
bool DataBase::remove(std::string name, std::map <std::string, T>& m)
{
    return m.erase(name) > 0;
}

//	returns true if it successfully removes something with that name
template<typename T>
bool DataBase::remove(std::string name, QDate date, std::map<QDate, std::vector<T>>& m)
{
    if (m.count(date) == 0) return false;

    auto& v = m[date];
    for (auto it = v.begin(); it != v.end(); it++) {
        if (it->name() == name) {
            v.erase(it);
            return true;
        }
    }

    return false;
}

bool DataBase::remove_foodtype(std::string name) { return remove(name, foods); }

bool DataBase::remove_exercisetype(std::string name) { return remove(name, exercises); }

bool DataBase::remove_recipe(std::string name) { return remove(name, recipes); }

bool DataBase::remove_meal(std::string name) { return remove(name, current_meals); }

bool DataBase::remove_food(std::string name, QDate day) 
    { return remove<Food>(name, day, food_history); }

bool DataBase::remove_exercise(std::string name, QDate day) 
    { return remove<Exercise>(name, day, exercise_history); }

std::string lower(const std::string& s)
{
    std::string cpy = s;

    for (char& c : cpy) c = std::tolower(c);

    return cpy;
}

//  could be done better by using that map iterates over keys in order?
template<typename T>
std::vector<T> DataBase::Tvec_name_contains(std::string s, const std::map<std::string, T>& m)
{
    std::string l = lower(s);
    std::vector<T> ret;
    for (const auto& val : m) {
        std::string n = lower(val.first);
        if (n.find(l) != std::string::npos) ret.push_back(val.second);
    }

    return ret;
}

std::vector<FoodType> DataBase::foodtype_name_contains(std::string s) 
    { return Tvec_name_contains(s, foods);}

std::vector<Recipe> DataBase::recipe_name_contains(std::string s)
    { return Tvec_name_contains(s, recipes); }

std::vector<ExerciseType> DataBase::exercisetype_name_contains(std::string s)
    { return Tvec_name_contains(s, exercises); }

std::vector<Meal> DataBase::meal_name_contains(std::string s)
    { return Tvec_name_contains(s, current_meals); }

DataCurrent& DataBase::today() { return data_today; }