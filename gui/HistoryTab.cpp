#include "HistoryTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

HistoryTab::HistoryTab(DataBase& db, QWidget* parent) 
	: QWidget(parent), database(db), series_one(nullptr), series_two(nullptr)
{
	start_date_selector = new QDateEdit(QDate::currentDate().addDays(-7));
	start_date_selector->setCalendarPopup(true);

	end_date_selector = new QDateEdit(QDate::currentDate());
	end_date_selector->setCalendarPopup(true);
	end_date_selector->setMaximumDate(QDate::currentDate());

	QStringList cdt(CHART_DATA_TYPES.cbegin(),CHART_DATA_TYPES.cend());
	data_one_selector = new QComboBox;
	data_one_selector->addItems(cdt);
	data_one_selector->setCurrentIndex(1);

	data_two_selector = new QComboBox;
	data_two_selector->addItems(cdt);
	data_two_selector->setCurrentIndex(2);

	chart = new QChart;

	x_axis = new QDateTimeAxis;
	x_axis->setFormat("dd.MM");
	chart->addAxis(x_axis,Qt::AlignBottom);
	update_x_axis();

	y1_axis = new QValueAxis;
	chart->addAxis(y1_axis, Qt::AlignLeft);

	y2_axis = new QValueAxis;
	chart->addAxis(y2_axis, Qt::AlignRight);

	update_y1_axis();
	update_y2_axis();

	chart_view = new QChartView(chart);

	QHBoxLayout* layout_selectors = new QHBoxLayout;
	layout_selectors->addWidget(new QLabel("From: "));
	layout_selectors->addWidget(start_date_selector, 0, Qt::AlignLeft);
	layout_selectors->addWidget(new QLabel("To: "));
	layout_selectors->addWidget(end_date_selector,0, Qt::AlignLeft);
	layout_selectors->addWidget(new QLabel("            "), 0, Qt::AlignLeft);
	layout_selectors->addWidget(new QLabel("Show: "), 0, Qt::AlignLeft);
	layout_selectors->addWidget(data_one_selector, 0, Qt::AlignLeft);
	layout_selectors->addWidget(data_two_selector, 1, Qt::AlignLeft);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(layout_selectors);
	layout->addWidget(chart_view, 1);

	setLayout(layout);
	chart_view->setRenderHint(QPainter::Antialiasing);
	//chart->setAnimationOptions(QChart::SeriesAnimations);

	QObject::connect(start_date_selector, &QDateEdit::userDateChanged,
		this, [this](QDate date) {update_x_axis();});
	QObject::connect(end_date_selector, &QDateEdit::userDateChanged,
		this, [this](QDate date) {update_x_axis();});

	QObject::connect(data_one_selector, &QComboBox::currentTextChanged, 
		this, [this](const QString&) {update_y1_axis();});
	QObject::connect(data_two_selector, &QComboBox::currentTextChanged,
		this, [this](const QString&) {update_y2_axis();});
}

void HistoryTab::update_x_axis()
{
	QDate start = start_date_selector->date();
	QDate end = end_date_selector->date();

	x_axis->setMin(QDateTime(start, QTime(0, 0)));
	x_axis->setMax(QDateTime(start, QTime(0, 0)));
}


QLineSeries* HistoryTab::update_y_axis(QValueAxis* y_axis, std::string selection)
{
	QDate start = start_date_selector->date();
	QDate end = end_date_selector->date();

	QLineSeries* data;
	if (selection == CHART_DATA_TYPES[0]) {
		data = new QLineSeries;
	}
	else if (selection == CHART_DATA_TYPES[1]) {
		data = fetch_weight(start, end);
		data->setName(CHART_DATA_TYPES[1]);
	}
	else if (selection == CHART_DATA_TYPES[2]) {
		data = fetch_cal_eaten(start, end);
		data->setName(CHART_DATA_TYPES[2]);
	}
	else if (selection == CHART_DATA_TYPES[3]) {
		data = fetch_cal_diff(start, end);
		data->setName(CHART_DATA_TYPES[3]);
	}
	else if (selection == CHART_DATA_TYPES[4]) {
		data = fetch_cal_ex(start, end);
		data->setName(CHART_DATA_TYPES[4]);
	}
	else {
		return nullptr; //	what happened??
	}

	chart->addSeries(data);
	data->attachAxis(y_axis);
	data->attachAxis(x_axis);
	data->setPointsVisible(true);

	auto l = data->points();
	if (!l.isEmpty()) {
		double max = std::max_element(l.begin(), l.end(),
			[](QPointF one, QPointF two) { return one.y() < two.y(); })->y();
		double min = std::min_element(l.begin(), l.end(),
			[](QPointF one, QPointF two) { return one.y() < two.y(); })->y();
		y_axis->setRange(min, max);
	}

	return data;
}

void HistoryTab::update_y1_axis()
{
	if(series_one) chart->removeSeries(series_one);
	delete series_one;
	series_one = update_y_axis(y1_axis, data_one_selector->currentText().toStdString());
}

void HistoryTab::update_y2_axis()
{
	if(series_two) chart->removeSeries(series_two);
	delete series_two;
	series_two = update_y_axis(y2_axis, data_two_selector->currentText().toStdString());
}

QLineSeries* HistoryTab::fetch_weight(QDate start, QDate end)
{
	QLineSeries* data = new QLineSeries;

	auto vec = database.weights_range(start, end);
	for (const auto& p : vec) {
		QDate date = p.first;
		data->append(QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch(), p.second);
	}

	return data;
}

QLineSeries* HistoryTab::fetch_cal_eaten(QDate start, QDate end)
{
	QLineSeries* data = new QLineSeries;

	auto vec = database.food_range(start, end);
	for (const auto& p : vec) {
		QDate date = p.first;
		CAL_T sum = 0;
		for (const Food& f : p.second) {
			sum += f.calories();
		}

		data->append(QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch(), sum);
	}

	return data;
}

QLineSeries* HistoryTab::fetch_cal_diff(QDate start, QDate end)
{
	QLineSeries* data = new QLineSeries;

	auto vec = database.food_range(start, end);
	for (const auto& p : vec) {
		QDate date = p.first;
		CAL_T sum = 0;
		for (const Food& f : p.second) {
			sum += f.calories();
		}

		std::vector<Exercise> v_ex= database.exercise_on(date);
		CAL_T ex_sum = 0;
		for (const Exercise& ex : v_ex) {
			sum += ex.calories();
		}

		data->append(QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch(), sum  - 1500 - ex_sum);
		// TODO load calorie usage from settings
	}

	return data;
}

QLineSeries* HistoryTab::fetch_cal_ex(QDate start, QDate end)
{
	QLineSeries* data = new QLineSeries;

	auto vec = database.exercise_range(start, end);
	for (const auto& p : vec) {
		QDate date = p.first;
		CAL_T sum = 0;
		for (const Exercise& ex : p.second) {
			sum += ex.calories();
		}

		data->append(QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch(), sum);
	}

	return data;
}