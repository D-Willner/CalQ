#include "WeightChart.h"
#include <qdatetimeaxis.h>
#include <QValueAxis>


WeightChart::WeightChart(const std::vector<std::pair<QDate, BODYWEIGHT_T>>& v, QWidget* parent)
	: QChartView(parent)
{
	data = new QLineSeries;
	data->setPointsVisible();

	chart = new QChart;
	chart->addSeries(data);
	chart->legend()->hide();

	y_axis = new QValueAxis;
	x_axis = new QDateTimeAxis;
	x_axis->setFormat("dd.MM");


	chart->addAxis(y_axis, Qt::AlignLeft);
	chart->addAxis(x_axis, Qt::AlignBottom);

	data->attachAxis(x_axis);
	data->attachAxis(y_axis);

	add(v);

	update_axes();

	setChart(chart);
	chart->setTitle("Weight history:");


	setRenderHint(QPainter::Antialiasing);
}

void WeightChart::update_axes()
{
	update_date_range();
	update_value_range();
	update_ticks();
}

void WeightChart::update_ticks()
{
	long long msecs = (x_axis->max() - x_axis->min()).count();
	long long days = msecs / (1000 * 60 * 60 * 24);
	x_axis->setTickCount(days + 1);
}

void WeightChart::update_date_range()
{
	x_axis->setMax(QDateTime(QDate::currentDate(), QTime(0, 0)));
	QDate min_date;
	auto list = data->points();
	if (list.size() == 0) {
		QDate td = QDate::currentDate();
		min_date = td.addDays(-7);
	}
	else {
		min_date = QDateTime::fromMSecsSinceEpoch(std::reduce(list.begin(), list.end(), list[0],
			[](QPointF one, QPointF two)
			{ return one.x() < two.x() ? one : two;}).x()).date();
	}
	x_axis->setMin(QDateTime(min_date, QTime(0, 0)));
}

void WeightChart::update_value_range()
{
	BODYWEIGHT_T minw = 0;
	auto list = data->points();
	if (list.size() > 0) {
		minw = std::reduce(list.begin(), list.end(), list[0],
			[](QPointF one, QPointF two)
			{ return one.y() < two.y() ? one : two; }).y();
	}
	BODYWEIGHT_T maxw = 100;
	if (list.size() > 0) {
		maxw = std::reduce(list.begin(), list.end(), list[0],
			[](QPointF one, QPointF two)
			{ return one.y() > two.y() ? one : two; }).y();
	}
	y_axis->setRange(minw, maxw);
}

WeightChart::WeightChart(QWidget* parent) : WeightChart({},parent) {}

void WeightChart::add(const std::vector<std::pair<QDate, BODYWEIGHT_T>>& v)
{
	//if (data == nullptr) throw new std::exception("WeightChart data not initialized");

	for (const std::pair<QDate, BODYWEIGHT_T>& p : v) {
		add(p.second, p.first);
	}
}

void WeightChart::add(BODYWEIGHT_T w, QDate date)
{
	QDateTime dt = QDateTime(date, QTime(0,0));

	QList<QPointF> pts = data->points();
	for (QPointF& pt : pts) {
		if (pt.x() == dt.toMSecsSinceEpoch()) {
			data->remove(pt);
			break;
		}
	}
	

	QDateTime date_time = QDateTime(date, QTime(0,0));
	data->append(date_time.toMSecsSinceEpoch(), w);

	update_axes();	
}

void WeightChart::add(std::pair<QDate, BODYWEIGHT_T> p)
{
	add(p.second,p.first);
}

void WeightChart::remove(QDate date)
{
	QDateTime dt = QDateTime(date, QTime(0, 0));
	QList<QPointF> pts = data->points();
	for (QPointF& pt : pts) {
		if (pt.x() == dt.toMSecsSinceEpoch()) {
			data->remove(pt);
			return;
		}
	}
	update_axes();
}