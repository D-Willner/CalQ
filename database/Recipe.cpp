#include "Recipe.h"

Recipe::Recipe(const Food& f) : Recipe(f.name() + "*", {f}) {}

Recipe::Recipe(std::string nm, const std::vector<Food>& ing) : recipe_name(nm), ingredients(ing) {}

Recipe::Recipe(std::string nm, const std::initializer_list<Food>& ing) : recipe_name(nm), ingredients(ing) {}


const std::vector<Food> Recipe::get_ingredients() const { return ingredients; }

/*
Recipe Recipe::operator* (int percentage) const
{
	std::vector<Food> nfoods = ingredients;
	for (const Food& f : ingredients)
		nfoods.push_back(f*percentage);

	return Recipe(recipe_name(), nfoods);
}
*/

Recipe& Recipe::operator*= (int percentage)
{
	for (Food& f : ingredients)
		f *= percentage;

	return *this;
}

Food Recipe::as_food() const
{
	WEIGHT_T total_weight = 0;
	CAL_T total_cal = 0;
	WEIGHT_T total_prot = 0;
	WEIGHT_T total_carb = 0;
	WEIGHT_T total_fat = 0;

	for (const Food& f : ingredients) {
		total_weight += f.grams();
		total_cal += f.calories();
		total_prot += f.protein();
		total_carb += f.carbs();
		total_fat += f.fats();
	}
	if (total_weight == 0) total_weight = 100;

	return Food(FoodType(recipe_name, total_cal * 100 / total_weight, total_prot * 100 / total_weight,
						 total_carb * 100 / total_weight, total_fat * 100 / total_weight), total_weight);

}


QJsonObject Recipe::to_json() const
{
	QJsonArray fds;
	for (const Food& f : ingredients) 
		fds.push_back(f.to_json());
	
	QJsonObject jo;
	jo["name"] = recipe_name.c_str();
	jo["ingredients"] = fds;

	return jo;
}


Recipe Recipe::from_json(const QJsonObject& jo)
{
	if (!jo.contains("name") || !jo["name"].isString())
		throw new std::exception("Recipe Json conversion invalid data");
	std::string nm = jo["name"].toString().toStdString();

	if (!jo.contains("ingredients") || !jo["ingredients"].isArray())
		throw new std::exception("Recipe Json conversion invalid data");
	std::vector<Food> ing;

	for (const QJsonValue& val : jo["ingredients"].toArray()) {
		if(!val.isObject())
			throw new std::exception("Recipe Json conversion invalid data");
		ing.push_back(Food::from_json(val.toObject()));
	}

	return Recipe(nm, ing);
}

std::string Recipe::name() const { return recipe_name;  }