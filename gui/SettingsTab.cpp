#include "SettingsTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include "InitializeDialog.h"

SettingsTab::SettingsTab(Settings& s, QWidget* parent) : QWidget(parent), settings(s)
{
	QPushButton* initialize_btn = new QPushButton("Initialize");

	target_weight_sb = new QDoubleSpinBox;
	target_weight_sb->setRange(0, 500);
	target_weight_sb->setValue(s.get_target_weight());

	starting_weight_sb = new QDoubleSpinBox;
	starting_weight_sb->setRange(0, 500);
	starting_weight_sb->setValue(s.get_starting_weight());

	calorie_target_sb = new QSpinBox();
	calorie_target_sb->setRange(0, 10000);
	calorie_target_sb->setValue(s.get_calorie_target());

	cal_burn_daily_sb = new QSpinBox();
	cal_burn_daily_sb->setRange(0, 10000);
	cal_burn_daily_sb->setValue(s.get_cal_burn());

	AI_enable_box = new QCheckBox;
	AI_enable_box->setCheckState(s.is_enabled_AI() ? Qt::Checked : Qt::Unchecked);

	accept_btn = new QPushButton("Accept");
	deny_btn = new QPushButton("Cancel");

	QFormLayout* form_layout = new QFormLayout;
	form_layout->addRow("Target Weight:", target_weight_sb);
	form_layout->addRow("Starting Weight:", starting_weight_sb);
	form_layout->addRow("Calorie Target:", calorie_target_sb);
	form_layout->addRow("Daily calories burned:", cal_burn_daily_sb);
	form_layout->addRow("Enable AI search:", AI_enable_box);

	QHBoxLayout* form_upper_layout = new QHBoxLayout;
	form_upper_layout->addLayout(form_layout);
	form_upper_layout->addStretch(2);
	//form_upper_layout->addSpacerItem(new QSpacerItem(100, 100, QSizePolicy::Minimum, QSizePolicy::Minimum));
	//form_upper_layout->addSpacing(700);

	QHBoxLayout* btn_layout = new QHBoxLayout;
	btn_layout->addWidget(accept_btn, 0, Qt::AlignLeft);
	btn_layout->addWidget(deny_btn, 1, Qt::AlignLeft);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(initialize_btn, 0, Qt::AlignLeft);
	layout->addLayout(form_upper_layout);
	layout->addLayout(btn_layout,1);
	layout->addStretch(1);

	QObject::connect(accept_btn, &QPushButton::clicked,
		this, [this]() {set();});
	QObject::connect(deny_btn, &QPushButton::clicked,
		this, [this]() {fetch();});
	QObject::connect(initialize_btn, &QPushButton::clicked,
		this, [=]() {
			InitializeDialog* init_dialog = new InitializeDialog(settings, this);
			QObject::connect(init_dialog, &QDialog::accepted, this, [this, init_dialog]() {
				this->fetch();
				init_dialog->deleteLater();
			});
			init_dialog->open();
	});
}

void SettingsTab::set()
{
	settings.set_target_weight(target_weight_sb->value());
	settings.set_starting_weight(starting_weight_sb->value());
	settings.set_calorie_target(calorie_target_sb->value());
	settings.set_cal_burn(cal_burn_daily_sb->value());
	settings.enable_AI(AI_enable_box->checkState() == Qt::Checked);
}

void SettingsTab::fetch()
{
	target_weight_sb->setValue(settings.get_target_weight());
	starting_weight_sb->setValue(settings.get_starting_weight());
	calorie_target_sb->setValue(settings.get_calorie_target());
	cal_burn_daily_sb->setValue(settings.get_cal_burn());
	AI_enable_box->setCheckState(settings.is_enabled_AI() ? Qt::Checked : Qt::Unchecked);
}