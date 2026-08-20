#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qdialog.h"
#include "database/settings.h"

//  Class asks user for some information to then initialize starting_weigth, target_weight, starting_date, calorie_target, cal_burn_daily
class InitializeDialog :
    public QDialog
{
    Q_OBJECT

private:
	Settings& settings;

	enum GENDER { MALE, FEMALE, OTHER };

	enum ACTIVITY_LEVEL { 
        SEDENTARY           = 0, 
        LIGHTLY_ACTIVE      = 1, 
        MODERATELY_ACTIVE   = 2, 
        VERY_ACTIVE         = 3, 
        EXTRA_ACTIVE        = 4
    };

    static constexpr double ACTIVITY_MULTIPLIERS[5] = { 1.2, 1.375, 1.55, 1.725, 1.9 };
    
    static CAL_T BMR(BODYWEIGHT_T weight, int age, int height, InitializeDialog::GENDER gender);

    static CAL_T TDEE(BODYWEIGHT_T weight, int age, int height, InitializeDialog::GENDER gender, InitializeDialog::ACTIVITY_LEVEL activity_level);

public:
    InitializeDialog(Settings& s, QWidget* parent = nullptr);
};

