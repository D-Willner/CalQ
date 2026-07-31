#include "CalorieDisplay.h"
#include <QPainter>

CalorieDisplay::CalorieDisplay(QWidget* parent) : QWidget(parent)
{
	name_label = new QLabel("Calories:");

	draw_area = new CalorieBars;
	draw_area->setMinimumSize(200, 100);

	
	layout = new QVBoxLayout();
	layout->addWidget(name_label, 0, Qt::AlignHCenter);
	layout->addWidget(draw_area);

	setLayout(layout);
}

CAL_T CalorieDisplay::get_target_calories() const { return calorie_target; }
CAL_T CalorieDisplay::get_consumed_calories() const { return calories_consumed; }
CAL_T CalorieDisplay::get_exercised_calories() const { return calories_exercised; }

void CalorieDisplay::set_target_calories(CAL_T c)
{ 
	calorie_target = c; 
	draw_area->set_target_calories(c);
}

void CalorieDisplay::set_consumed_calories(CAL_T c)
{
	calories_consumed = c;
	draw_area->set_consumed_calories(c);
}

void CalorieDisplay::set_exercised_calories(CAL_T c)
{
	calories_exercised = c;
	draw_area->set_exercised_calories(c);
}

void CalorieDisplay::add_target_calories(CAL_T c)
{
	calorie_target += c;
	draw_area->set_target_calories(calorie_target);
}

void CalorieDisplay::add_consumed_calories(CAL_T c)
{
	calories_consumed += c;
	draw_area->set_consumed_calories(calories_consumed);
}

void CalorieDisplay::add_exercised_calories(CAL_T c)
{
	calories_exercised += c;
	draw_area->set_exercised_calories(calories_exercised);
}