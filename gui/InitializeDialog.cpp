#include "InitializeDialog.h"
#include <QFormLayout>
#include <QBoxLayout>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "ToolTipLabel.h"

InitializeDialog::InitializeDialog(Settings& s, QWidget* parent) : QDialog(parent), settings(s)
{
	QPushButton* accept_btn = new QPushButton("Accept");
	QPushButton* cancel_btn = new QPushButton("Cancel");
	QPushButton* calculate_btn = new QPushButton("Calculate");
	calculate_btn->setAutoDefault(false);

	QDoubleSpinBox* weight_spin = new QDoubleSpinBox;
	weight_spin->setRange(0, 500);
	weight_spin->setSingleStep(0.5);

	QSpinBox* age_spin = new QSpinBox;
	age_spin->setRange(0, 120);

	QSpinBox* height_spin = new QSpinBox;
	height_spin->setRange(0, 300);

	QComboBox* gender_combo = new QComboBox;
	gender_combo->addItems({ "Male", "Female", "Other" });

	QComboBox* activity_level_combo = new QComboBox;
	activity_level_combo->addItems({ "Sedentary", "Lightly Active", "Moderately Active", "Very Active", "Extra Active" });

	QDoubleSpinBox* calorie_deficit_spin = new QDoubleSpinBox;
	calorie_deficit_spin->setRange(0, 2000);
	calorie_deficit_spin->setSingleStep(50);

	QLineEdit* calorie_target_edit = new QLineEdit;
	QLineEdit* calorie_expenditure_edit = new QLineEdit;

	QFormLayout* form_layout = new QFormLayout;
	form_layout->addRow("Starting Weight (kg):", weight_spin);
	form_layout->addRow("Age:", age_spin);
	form_layout->addRow("Height (cm):", height_spin);
	form_layout->addRow("Gender:", gender_combo);
	form_layout->addRow("Activity Level:", activity_level_combo);
	form_layout->addRow("Calorie Deficit (kcal):", calorie_deficit_spin);

	QHBoxLayout* calc_layout = new QHBoxLayout;
	calc_layout->addWidget(new ToolTipLabel("These are editable.", "Results:"), 1, Qt::AlignLeft);
	calc_layout->addWidget(calculate_btn,0,Qt::AlignRight);

	QFormLayout* result_layout = new QFormLayout;
	result_layout->addRow("Predicted Calorie Expenditure (kcal):", calorie_expenditure_edit);
	result_layout->addRow("Calorie Target (kcal):", calorie_target_edit);

	QHBoxLayout* button_layout = new QHBoxLayout;
	button_layout->addWidget(accept_btn);
	button_layout->addWidget(cancel_btn, 1, Qt::AlignLeft);

	QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->addWidget(new ToolTipLabel("Please enter your starting weight, age, gender, activity level, and desired calorie deficit to initialize the application.",
		"Initialization Settings:"), 0, Qt::AlignLeft);
	main_layout->addLayout(form_layout);
	main_layout->addLayout(calc_layout);
	main_layout->addLayout(result_layout);
	main_layout->addLayout(button_layout);

	//	Button signals
	QObject::connect(accept_btn, &QPushButton::clicked, this, [=]() {
		settings.set_calorie_target(calorie_target_edit->text().toDouble());
		settings.set_cal_burn(calorie_expenditure_edit->text().toDouble());
		settings.set_starting_weight(weight_spin->value());
		accept();
	});

	QObject::connect(cancel_btn, &QPushButton::clicked, this, [=]() {
		reject();
	});

	QObject::connect(calculate_btn, &QPushButton::clicked, this, [=]() {
		BODYWEIGHT_T weight = static_cast<BODYWEIGHT_T>(weight_spin->value());
		int age = age_spin->value();
		int height = height_spin->value();
		GENDER gender = static_cast<InitializeDialog::GENDER>(gender_combo->currentIndex());
		ACTIVITY_LEVEL activity_level = static_cast<InitializeDialog::ACTIVITY_LEVEL>(activity_level_combo->currentIndex());
		CAL_T calorie_deficit = calorie_deficit_spin->value();

		//	Calculate results
		CAL_T tdee = TDEE(weight, age, height, gender, activity_level);
		CAL_T calorie_target = tdee - calorie_deficit;

		//	Update result fields
		calorie_expenditure_edit->setText(QString::number(tdee));
		calorie_target_edit->setText(QString::number(calorie_target));
	});

	setFocusPolicy(Qt::ClickFocus);
	setAttribute(Qt::WA_DeleteOnClose);
}

CAL_T InitializeDialog::BMR(BODYWEIGHT_T weight, int age, int height, InitializeDialog::GENDER gender) {
	// Using the Mifflin-St Jeor Equation for BMR calculation	
	///*
	if (gender == InitializeDialog::MALE) {
		return 10 * weight + 6.25 * height - 5 * age + 5;
	} else {
		return 10 * weight + 6.25 * height - 5 * age - 161;
	}
	//*/

	//	Alternatively, using the Harris-Benedict Equation
	/*
	if (gender == InitializeDialog::MALE) {
		return 13.75 * weight + 5.003 * height - 6.755 * age + 66.5;
	} else {
		return 9.563 * weight + 1.85 * height - 4.676 * age + 655.1;
	}
	*/
}

CAL_T InitializeDialog::TDEE(BODYWEIGHT_T weight, int age, int height, InitializeDialog::GENDER gender, InitializeDialog::ACTIVITY_LEVEL activity_level) {
	CAL_T bmr = BMR(weight, age, height, gender);
	return bmr * InitializeDialog::ACTIVITY_MULTIPLIERS[activity_level];
}