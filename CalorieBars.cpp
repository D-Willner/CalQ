#include "CalorieBars.h"
#include <QPainter>


CalorieBars::CalorieBars(QWidget* parent) : QWidget(parent), 
	calories_consumed(0), calories_exercised(0), calorie_target(0)
{
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
}  

void CalorieBars::paintEvent(QPaintEvent* e)
{

	CAL_T max = std::max(calories_consumed, calorie_target + calories_exercised);
	max = std::max(max, 1000.0);
	int width_consumed = (calories_consumed) / max * (this->width() - 2 * MARGIN);
	int width_target = (calorie_target) / max * (this->width() - 2 * MARGIN);
	int width_exercised = (calories_exercised) / max * (this->width() - 2 * MARGIN);
	int height_bar = (this->height() - MARGIN) / 2 - MARGIN;

	QPainter painter(this);
	painter.setFont(QFont("Helvetica", FONT_SIZE, QFont::Bold));
	painter.fillRect(MARGIN, MARGIN, width_consumed, height_bar, LIGHT_GREEN);
	QPen pen(Qt::darkGreen);
	painter.setPen(pen);
	painter.drawRect(MARGIN, MARGIN, width_consumed, height_bar);
	pen.setColor(Qt::black);
	painter.setPen(pen);
	if (width_consumed > 30) {
		painter.drawText(2 * MARGIN, MARGIN + height_bar / 2 + FONT_SIZE / 2, QString::number(calories_consumed));
	}
	else {
		painter.drawText(2 * MARGIN + width_consumed, MARGIN + height_bar / 2 + FONT_SIZE / 2, QString::number(calories_consumed));
	}

	painter.fillRect(MARGIN, 2 * MARGIN + height_bar, width_target, height_bar, LIGHT_BLUE);
	pen.setColor(Qt::darkBlue);
	painter.setPen(pen);
	painter.drawRect(MARGIN, 2 * MARGIN + height_bar, width_target, height_bar);

	painter.fillRect(MARGIN + width_target, 2 * MARGIN + height_bar, width_exercised, height_bar, LIGHT_PURPLE);
	pen.setColor(Qt::darkBlue);
	painter.setPen(pen);
	painter.drawRect(MARGIN + width_target, 2 * MARGIN + height_bar, width_exercised, height_bar);
	pen.setColor(Qt::black);
	painter.setPen(pen);
	if (calories_exercised > 0) {
		painter.drawText(2 * MARGIN, 2 * MARGIN + height_bar + height_bar / 2 + FONT_SIZE / 2,
			QString::number(calorie_target) + " + " + QString::number(calories_exercised));
	}
	else {
		painter.drawText(2 * MARGIN, 2 * MARGIN + height_bar + height_bar / 2 + FONT_SIZE / 2, QString::number(calorie_target));
	}
}

void CalorieBars::set_target_calories(CAL_T c) { calorie_target = c; update(); }
void CalorieBars::set_consumed_calories(CAL_T c) { calories_consumed = c; update(); }
void CalorieBars::set_exercised_calories(CAL_T c) { calories_exercised = c; update(); }