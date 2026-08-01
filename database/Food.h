#pragma once
#include "FoodType.h"

#include <qjsonobject.h>

class Food
{
private:

protected:
	FoodType type;
	WEIGHT_T weight;	//	in grams

	void set_type(const FoodType& t);
	explicit Food();

public:
	explicit Food(const FoodType& t, WEIGHT_T w);
	explicit Food(const FoodType& t);

	//	w must not be 0
	explicit Food(std::string name, CAL_T cals = 0, 
		WEIGHT_T prot = 0, WEIGHT_T carbs = 0, WEIGHT_T fats = 0, WEIGHT_T w = 100);

	std::string name() const;

	FoodType food_type() const;

	CAL_T calories() const;
	WEIGHT_T protein() const;
	WEIGHT_T carbs() const;
	WEIGHT_T fats() const;

	WEIGHT_T grams() const;
	void set_weight(WEIGHT_T w);

	Food& operator*= (double percentage);
	Food operator* (double percentage) const;

	QJsonObject to_json() const;
	static Food from_json(const QJsonObject& jo);
};

