#pragma once
#include <memory>
#include "Recipe.h"
#include <qjsonobject.h>

class Meal : public Food
{
private:
    double left;   //  in percent
public:
    explicit Meal(const Recipe& r, double l = 100, int count = 1);
    explicit Meal(const Food& f, double l = 100, int count = 1);

    double percentage_left() const;
    void set_percentage_left(double l);

    WEIGHT_T weight_left() const;
    void set_weight_left(WEIGHT_T w);

    QJsonObject to_json() const;
    static Meal from_json(const QJsonObject& jo);
};

