#include "Meal.h"

Meal::Meal(const Recipe& r, double l, int count) : Food(r.as_food()), left(l) 
{ 
	type.set_name(name() + "~" + std::to_string(count));
};

Meal::Meal(const Food& f, double l, int count) : Food(f), left(l) 
{
	type.set_name(name() + "~" + std::to_string(count));
};

double Meal::percentage_left() const { return left; }

void Meal::set_percentage_left(double l) { left = l; }

WEIGHT_T Meal::weight_left() const { return grams(); }

void Meal::set_weight_left(WEIGHT_T w) { left = w / grams(); }


QJsonObject Meal::to_json() const
{
	QJsonObject jo;
	jo["food"] = Food::to_json();
	jo["left"] = left;

	return jo;
}

Meal Meal::from_json(const QJsonObject& jo)
{
	if (!jo.contains("food") || !jo["food"].isObject())
		throw new std::exception("Meal Json conversion invalid data");
	Food f = Food::from_json(jo["food"].toObject());

	if (!jo.contains("left") || !jo["left"].isDouble())
		throw new std::exception("Meal Json conversion invalid data");
	int wt = jo["left"].toDouble();

	return Meal(f, wt);
}