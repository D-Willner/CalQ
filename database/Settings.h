#pragma once
#include <QDate>
#include <QJsonObject>

#include "Foodtype.h"
#include "ExerciseType.h"
#include "WeightEntry.h"

class Settings
{
private:
	BODYWEIGHT_T target_weight;
	BODYWEIGHT_T starting_weight;
	QDate starting_date;

	CAL_T calorie_target;
	CAL_T cal_burn_daily;

	bool AI_enabled;

	Settings() = default;
public:

	BODYWEIGHT_T get_target_weight() const;
	BODYWEIGHT_T get_starting_weight() const;
	QDate get_starting_date() const;

	CAL_T get_calorie_target() const;
	CAL_T get_cal_burn() const;

	bool is_enabled_AI() const;
	
	void set_target_weight(BODYWEIGHT_T w);
	void set_starting_weight(BODYWEIGHT_T w);
	void set_starting_date(QDate qd);

	void set_calorie_target(CAL_T c);
	void set_cal_burn(CAL_T c);

	void enable_AI(bool);

	static Settings from_json(const QJsonObject& jo);
	QJsonObject to_json() const;

	static Settings load();
	bool save() const;

	~Settings();
};

