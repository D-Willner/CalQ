#include "Settings.h"
#include <QFile>
#include <QDir>

BODYWEIGHT_T Settings::get_target_weight() const { return target_weight; }
BODYWEIGHT_T Settings::get_starting_weight() const { return starting_weight; }
QDate Settings::get_starting_date() const { return starting_date; }

CAL_T Settings::get_calorie_target() const { return calorie_target; }
CAL_T Settings::get_cal_burn() const { return cal_burn_daily; }

bool Settings::is_enabled_AI() const { return AI_enabled; }

void Settings::set_target_weight(BODYWEIGHT_T w) { target_weight = w; }
void Settings::set_starting_weight(BODYWEIGHT_T w) { starting_weight = w; }
void Settings::set_starting_date(QDate qd) { starting_date = qd; }

void Settings::set_calorie_target(CAL_T c) { calorie_target = c; }
void Settings::set_cal_burn(CAL_T c) { cal_burn_daily = c; }

void Settings::enable_AI(bool en) { AI_enabled = en; }

QJsonObject Settings::to_json() const
{
	QJsonObject jo;

	jo["target_weight"] = target_weight;
	jo["starting_weight"] = starting_weight;
	jo["starting_date"] = starting_date.toString();
	jo["calorie_target"] = calorie_target;
	jo["cal_burn_daily"] = cal_burn_daily;
	jo["AI_enabled"] = AI_enabled;

	return jo;
}

Settings Settings::from_json(const QJsonObject& jo)
{
	Settings s;

	s.target_weight = jo["target_weight"].toDouble(0.0);
	s.starting_weight = jo["starting_weight"].toDouble(0.0);
	s.starting_date = QDate::fromString(jo["starting_date"].toString(QDate::currentDate().toString()));
	s.calorie_target = jo["calorie_target"].toDouble(0.0);
	s.cal_burn_daily = jo["cal_burn_daily"].toDouble(0.0);
	s.AI_enabled = jo["AI_enabled"].toBool(true);

	return s;
}

bool Settings::save() const
{
	QFile file("resources/settings.json");
	if (!file.open(QFile::WriteOnly)) throw new std::exception("Could not open settings file");

	QJsonObject jo = to_json();
	QJsonDocument doc(jo);

	return file.write(doc.toJson());
}

Settings Settings::load()
{
	if (!QDir("./resources").exists()) {
		QDir().mkdir("resources");
	}

	QFile file("resources/settings.json");

	bool res;
	if (!file.exists()) res = file.open(QFile::ReadWrite);
	else res = file.open(QFile::ReadOnly);

	if (!res) throw new std::exception("Could not open settings file");

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (doc.isEmpty() || doc.isNull()) return Settings::from_json(QJsonObject());

	if (!doc.isObject()) throw new std::exception("Unexpected Json format");
	QJsonObject jo = doc.object();

	return Settings::from_json(jo);
}


Settings::~Settings()
{
	save();
}