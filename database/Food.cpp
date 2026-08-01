#include "Food.h"


Food::Food(const FoodType& t, WEIGHT_T w) : type(t), weight(w) {};
Food::Food(const FoodType& t) : type(t), weight(t.get_size()) {};
Food::Food() {};


Food::Food(std::string name, CAL_T cals,
	WEIGHT_T prot, WEIGHT_T carbs, WEIGHT_T fats, WEIGHT_T w)
	: Food(FoodType(name, cals*100/w, prot*100/w, carbs*100/w, fats*100/w), w) {}

std::string Food::name() const { return type.name(); }

FoodType Food::food_type() const
{
	return type;
}

CAL_T Food::calories() const { return type.get_cals() * weight / 100.0; }

WEIGHT_T Food::protein() const { return type.get_prots() * weight / 100.0; }

WEIGHT_T Food::carbs() const { return type.get_carbs() * weight / 100.0; }

WEIGHT_T Food::fats() const { return type.get_fats() * weight / 100.0; }

WEIGHT_T Food::grams() const { return weight; }

void Food::set_weight(WEIGHT_T w) { weight = w; }

void Food::set_type(const FoodType& t) { type = t; }

Food Food::operator* (double percentage) const
{
	Food f(*this);
	f *= percentage;
	return f;
}

Food& Food::operator*= (double percentage)
{
	set_weight(grams() * percentage);
	return *this;
}

QJsonObject Food::to_json() const
{
	QJsonObject jo;
	jo["type"] = type.to_json();
	jo["weight"] = weight;

	return jo;
}

Food Food::from_json(const QJsonObject& jo)
{
	if (!jo.contains("type") || !jo["type"].isObject())
		throw new std::exception("Food Json conversion invalid data");
	FoodType ft = FoodType::from_json(jo["type"].toObject());

	if (!jo.contains("weight") || !jo["weight"].isDouble())
		throw new std::exception("Food Json conversion invalid data");
	int wt = jo["weight"].toDouble();

	return Food(ft, wt);
}