#pragma once
#include "ExerciseType.h"
#include <QJsonObject>

typedef double TIME_T;

class Exercise
{
private:
	ExerciseType type;
	TIME_T dur;	//	in minutes

public:

	explicit Exercise(const ExerciseType& t, TIME_T d);

	std::string name() const;

	ExerciseType exercise_type() const;

	CAL_T calories() const;

	TIME_T duration() const;

	QJsonObject to_json() const;
	static Exercise from_json(const QJsonObject& jo);
};

