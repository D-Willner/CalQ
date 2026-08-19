#pragma once
#include <QJsonDocument>
#include <QJsonObject>

class SQLSettings
{
private:
	std::string server_address;
	std::string server_port;
	std::string database_name;
	std::string database_driver;

	std::string foodtype_table_key;
	//std::string recipe_table_key;
	//std::string meal_table_key;

	std::string name_key;
	std::string cal_key;
	std::string prot_key;
	std::string carb_key;
	std::string fat_key;
	std::string size_key;

public:
	SQLSettings();

	enum SETTINGS_KEY {
		SERVER_ADDRESS_KEY,
		SERVER_PORT_KEY,
		DATABASE_NAME_KEY,
		DATABASE_DRIVER_KEY,
		FOODTYPE_TABLE_KEY,
		//RECIPE_TABLE_KEY,
		//MEAL_TABLE_KEY,
		NAME_KEY,
		CAL_KEY,
		PROT_KEY,
		CARB_KEY,
		FAT_KEY,
		SIZE_KEY
	};

	static SQLSettings from_json(const QJsonObject& jo);
	QJsonObject to_json() const;

	void set_key(SETTINGS_KEY, std::string key_value);
	std::string get_key(SETTINGS_KEY) const;
};

