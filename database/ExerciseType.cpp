#include "ExerciseType.h"


ExerciseType::ExerciseType(std::string n, CAL_T c) : exercise_name(n), cals_per(c) {};

std::string ExerciseType::name() const { return exercise_name; }

CAL_T ExerciseType::calories_per_hour() const { return cals_per; }


QJsonObject ExerciseType::to_json() const
{
	QJsonObject jo;

	jo["exercise_name"] = exercise_name.c_str();
	jo["cals_per"] = cals_per;

	return jo;
}

ExerciseType ExerciseType::from_json(const QJsonObject& jo)
{
	if (!jo.contains("exercise_name") || !jo["exercise_name"].isString())
		throw new std::exception("ExerciseType Json conversion invalid data");

	if (!jo.contains("cals_per") || !jo["cals_per"].isDouble())
		throw new std::exception("ExerciseType Json conversion invalid data");

	return ExerciseType(jo["exercise_name"].toString().toStdString(), jo["cals_per"].toDouble());
}