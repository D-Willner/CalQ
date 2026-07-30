#pragma once
#include <string>
#include <QJsonObject>

typedef double WEIGHT_T;
typedef double CAL_T;

class FoodType
{
private:
	std::string food_name;

	WEIGHT_T standard_size;

	CAL_T cal_per;	//	calories per 100g
	WEIGHT_T prot_per;
	WEIGHT_T carb_per;
	WEIGHT_T fat_per;

	constexpr static int CAL_PROT = 4;	//	calories per gram
	constexpr static int CAL_CARB = 4;
	constexpr static int CAL_FAT = 9;

	constexpr static int calc_cal(WEIGHT_T p, WEIGHT_T c, WEIGHT_T f);

public:
	explicit FoodType(std::string n = "", CAL_T cal = 0, WEIGHT_T p = 0, WEIGHT_T c = 0, WEIGHT_T f = 0, double s = 100);

	std::string name() const;

	void set_name(std::string);

	WEIGHT_T get_size() const;

	CAL_T get_cals() const;
	WEIGHT_T get_prots() const;
	WEIGHT_T get_carbs() const;
	WEIGHT_T get_fats() const;

	QJsonObject to_json() const;
	static FoodType from_json(const QJsonObject& jo);
};

