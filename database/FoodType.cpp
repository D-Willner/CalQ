#include "FoodType.h"

FoodType::FoodType(std::string n, CAL_T cal, WEIGHT_T p, WEIGHT_T c, WEIGHT_T f, WEIGHT_T s) : food_name(n), cal_per(cal), prot_per(p), carb_per(c), fat_per(f), standard_size(s) {};

std::string FoodType::name() const { return food_name; }

void FoodType::set_name(std::string nm) { food_name = nm; }

WEIGHT_T FoodType::get_size() const { return standard_size; }

CAL_T FoodType::get_cals() const { return cal_per; }

WEIGHT_T FoodType::get_prots() const  { return prot_per; }
WEIGHT_T FoodType::get_carbs() const  { return carb_per; }
WEIGHT_T FoodType::get_fats() const { return fat_per; }

constexpr int FoodType::calc_cal(WEIGHT_T p, WEIGHT_T c, WEIGHT_T f)
{
	return p * CAL_PROT + c * CAL_CARB + f * CAL_FAT;
}


QJsonObject FoodType::to_json() const
{
	QJsonObject jo;

	jo["food_name"] = food_name.c_str();
	jo["cal_per"] = cal_per;
	jo["prot_per"] = prot_per;
	jo["carb_per"] = carb_per;
	jo["fat_per"] = fat_per;
	jo["standard_size"] = standard_size;

	return jo;
}

FoodType FoodType::from_json(const QJsonObject& jo)
{
	FoodType f;

	if (jo.contains("food_name") && jo["food_name"].isString())
		f.food_name = jo["food_name"].toString().toStdString();
	else
		throw new std::exception("Wrong JSON read!");

	if (jo.contains("cal_per") && jo["cal_per"].isDouble())
		f.cal_per = jo["cal_per"].toDouble();
	else
		throw new std::exception("Wrong JSON read!");

	if (jo.contains("prot_per") && jo["prot_per"].isDouble())
		f.prot_per = jo["prot_per"].toDouble();
	else
		throw new std::exception("Wrong JSON read!");

	if (jo.contains("carb_per") && jo["carb_per"].isDouble())
		f.carb_per = jo["carb_per"].toDouble();
	else
		throw new std::exception("Wrong JSON read!");

	if (jo.contains("fat_per") && jo["fat_per"].isDouble())
		f.fat_per = jo["fat_per"].toDouble();
	else
		throw new std::exception("Wrong JSON read!");

	if (jo.contains("standard_size") && jo["standard_size"].isDouble())
		f.standard_size = jo["standard_size"].toDouble();
	else
		throw new std::exception("Wrong JSON read!");

	return f;
}