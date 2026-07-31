#pragma once
#include "FoodType.h"
#include "Food.h"
#include <vector>
#include <initializer_list>
#include <QJsonObject>
#include <QJsonArray>

class Recipe 
{
private:
    std::string recipe_name;
    std::vector<Food> ingredients;

protected:

public:
    explicit Recipe(const Food& f);
    explicit Recipe(std::string nm, const std::vector<Food>& ing);
    explicit Recipe(std::string nm, const std::initializer_list<Food>& ing);

    const std::vector<Food> get_ingredients() const;

    //Recipe operator* (int percentage) const;
    Recipe& operator*= (int percentage);

    Food as_food() const;

    std::string name() const;

    QJsonObject to_json() const;
    static Recipe from_json(const QJsonObject& jo);
};

