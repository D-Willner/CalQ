#include "WeightEntry.h"

WeightEntry::WeightEntry(BODYWEIGHT_T w) : bodyweight(w) {}
WeightEntry::WeightEntry() : WeightEntry(INVALID) {}

void WeightEntry::set_weight(BODYWEIGHT_T w) { bodyweight = w; }

BODYWEIGHT_T WeightEntry::get_weight() { return bodyweight; }

WeightEntry WeightEntry::from_json(const QJsonObject& jo)
{
	if (!jo.contains("bodyweight") || !jo["bodyweight"].isDouble())
		throw new std::exception("Bodyweight Json read error");

	BODYWEIGHT_T w = jo["bodyweight"].toDouble();

	return WeightEntry(w);
}

QJsonObject WeightEntry::to_json() const
{
	QJsonObject jo;
	jo["bodyweight"] = bodyweight;

	return jo;
}