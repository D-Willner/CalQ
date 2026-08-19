#include "SQLSettings.h"

SQLSettings::SQLSettings() 
	: server_address("localhost"), server_port("5432"), database_name(""), database_driver("QPSQL"), 
	foodtype_table_key("FOODTYPES"), name_key("NAME"), cal_key("CALS"), prot_key("PROT"), 
	carb_key("CARBS"), fat_key("FATS"), size_key("SIZE")
{}

SQLSettings SQLSettings::from_json(const QJsonObject& jo)
{
	SQLSettings settings;

	if (jo.contains("server_address"))		settings.server_address		= jo["server_address"].toString().toStdString();
	if (jo.contains("server_port"))			settings.server_port		= jo["server_port"].toString().toStdString();
	if (jo.contains("database_name"))		settings.database_name		= jo["database_name"].toString().toStdString();
	if (jo.contains("database_driver"))		settings.database_driver	= jo["database_driver"].toString().toStdString();
	if (jo.contains("foodtype_table_key"))	settings.foodtype_table_key	= jo["foodtype_table_key"].toString().toStdString();
	if (jo.contains("name_key"))			settings.name_key			= jo["name_key"].toString().toStdString();
	if (jo.contains("cal_key"))				settings.cal_key			= jo["cal_key"].toString().toStdString();
	if (jo.contains("prot_key"))			settings.prot_key			= jo["prot_key"].toString().toStdString();
	if (jo.contains("carb_key"))			settings.carb_key			= jo["carb_key"].toString().toStdString();
	if (jo.contains("fat_key"))				settings.fat_key			= jo["fat_key"].toString().toStdString();
	if (jo.contains("size_key"))			settings.size_key			= jo["size_key"].toString().toStdString();

	return settings;
}

QJsonObject SQLSettings::to_json() const
{
	QJsonObject jo;

	jo["server_address"]		= QString::fromStdString(server_address);
	jo["server_port"]			= QString::fromStdString(server_port);
	jo["database_name"]			= QString::fromStdString(database_name);
	jo["database_driver"]		= QString::fromStdString(database_driver);
	jo["foodtype_table_key"]	= QString::fromStdString(foodtype_table_key);
	jo["name_key"]				= QString::fromStdString(name_key);
	jo["cal_key"]				= QString::fromStdString(cal_key);
	jo["prot_key"]				= QString::fromStdString(prot_key);
	jo["carb_key"]				= QString::fromStdString(carb_key);
	jo["fat_key"]				= QString::fromStdString(fat_key);
	jo["size_key"]				= QString::fromStdString(size_key);

	return jo;
}

void SQLSettings::set_key(SETTINGS_KEY key, std::string key_value)
{
	switch(key)
	{
	case SERVER_ADDRESS_KEY:
		server_address = key_value;
		break;
	case SERVER_PORT_KEY:
		server_port = key_value;
		break;
	case DATABASE_NAME_KEY:
		database_name = key_value;
		break;
	case DATABASE_DRIVER_KEY:
		database_driver = key_value;
		break;
	case FOODTYPE_TABLE_KEY:
		foodtype_table_key = key_value;
		break;
	case NAME_KEY:
		name_key = key_value;
		break;
	case CAL_KEY:
		cal_key = key_value;
		break;
	case PROT_KEY:
		prot_key = key_value;
		break;
	case CARB_KEY:
		carb_key = key_value;
		break;
	case FAT_KEY:
		fat_key = key_value;
	 break;
	case SIZE_KEY:
		size_key = key_value;
	 break;
	default:
		throw std::invalid_argument("Invalid settings key");
	}
}

std::string SQLSettings::get_key(SETTINGS_KEY key) const
{
	switch (key)
	{
	case SERVER_ADDRESS_KEY:
		return server_address;
	case SERVER_PORT_KEY:
		return server_port;
	case DATABASE_NAME_KEY:
		return database_name;
	case DATABASE_DRIVER_KEY:
		return database_driver;
	case FOODTYPE_TABLE_KEY:
		return foodtype_table_key;
	case NAME_KEY:
		return name_key;
	case CAL_KEY:
		return cal_key;
	case PROT_KEY:
		return prot_key;
	case CARB_KEY:
		return carb_key;
	case FAT_KEY:
		return fat_key;
	case SIZE_KEY:
		return size_key;
	default:
		throw std::invalid_argument("Invalid settings key");
	}
}