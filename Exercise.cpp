#include "Exercise.h"


Exercise::Exercise(const ExerciseType& t, TIME_T d) : type(t), dur(d) {};

std::string Exercise::name() const { return type.name(); }

CAL_T Exercise::calories() const { return (type.calories_per_hour() * dur) / 60; }

TIME_T Exercise::duration() const { return dur; }


QJsonObject Exercise::to_json() const
{
	QJsonObject jo;

	jo["type"] = type.to_json();
	jo["dur"] = dur;

	return jo;
}

Exercise Exercise::from_json(const QJsonObject& jo)
{
	if (!jo.contains("type") || !jo["type"].isObject())
		throw new std::exception("Exercise Json conversion invalid data");
	ExerciseType et = ExerciseType::from_json(jo["type"].toObject());

	if (!jo.contains("dur") || !jo["dur"].isDouble())
		throw new std::exception("Exercise Json conversion invalid data");
	TIME_T d = jo["dur"].toDouble();

	return Exercise(et, d);
}


ExerciseType Exercise::exercise_type() const
{
	return type;
}