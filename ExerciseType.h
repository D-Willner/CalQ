#pragma once
#include <string>
#include <QJsonObject>
#include "Food.h"

class ExerciseType
{
private:
	std::string exercise_name;
	CAL_T cals_per;	//	calories burned per hour

public:
	explicit ExerciseType(std::string n, CAL_T c = 0);

	std::string name() const;
	CAL_T calories_per_hour() const;

	QJsonObject to_json() const;
	static ExerciseType from_json(const QJsonObject& jo);
};

