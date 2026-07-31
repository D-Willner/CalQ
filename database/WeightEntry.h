#pragma once
#include <QJSonObject>

typedef double BODYWEIGHT_T;

class WeightEntry
{
private:
	BODYWEIGHT_T bodyweight;

	enum{INVALID = -1};

public:
	WeightEntry(BODYWEIGHT_T w);
	WeightEntry();

	void set_weight(BODYWEIGHT_T w);

	BODYWEIGHT_T get_weight();

	QJsonObject to_json() const;
	static WeightEntry from_json(const QJsonObject& jo);
};

