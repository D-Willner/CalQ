#include "SQLDatabase.h"


SQLDatabase::SQLDatabase(std::string db_name, std::string db_driver, std::string address, std::string port) 
	: database_name(db_name), database_driver(db_driver), server_address(address), server_port(port)
{
	//	maybe connect here, probably not
}

SQLDatabase::SQLDatabase(const SQLDatabase& other)
	: qsql_database(other.qsql_database), database_name(other.database_name), 
	database_driver(other.database_driver), server_address(other.server_address), 
	server_port(other.server_port)
{}

bool SQLDatabase::connect(std::string db_name, std::string address)
{
	qsql_database = QSqlDatabase::addDatabase(QString::fromStdString(database_driver));

	qsql_database.setDatabaseName(QString::fromStdString(db_name));
	qsql_database.setConnectOptions("hostaddr=" + QString::fromStdString(address) + " "
		+"port=" + QString::fromStdString(server_port));

	database_name = db_name;
	server_address = address;

	return qsql_database.open();
}

bool SQLDatabase::connect(std::string db_name) { return connect(db_name, server_address); }

bool SQLDatabase::connect() { return connect(database_name, server_address); }

bool SQLDatabase::is_connected()
{
	return qsql_database.isOpen();
}

bool SQLDatabase::initialize_db()
{
	
	QSqlQuery check_query;
	check_query.exec(QString::fromStdString("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = '") 
		+ QString::fromStdString(foodtype_table_key).toLower() + "';");
	bool table_exists = check_query.size() > 0;
	if (table_exists) return true;

	QSqlQuery create_query;
	return create_query.exec(QString::fromStdString("CREATE TABLE " + foodtype_table_key + " ("
		+ name_key	+ " " + "VARCHAR(80)" + ", "
		+ cal_key	+ " " + "REAL" + ", "
		+ prot_key	+ " " + "REAL" + ", "
		+ carb_key	+ " " + "REAL" + ", "
		+ fat_key	+ " " + "REAL" + ", "
		+ size_key	+ " " + "REAL"
		+ ");"
	));
}

bool SQLDatabase::disconnect()
{
	qsql_database.close();
	return true;
}

/*
template<typename T>
std::string SQL_database::key_from_type(const T&)
{
	if (typeid(T) == typeid(FoodType)) return foodtype_table_key;
	else if (typeid(T) == typeid(Recipe)) return recipe_table_key;
	else if (typeid(T) == typeid(Meal)) return meal_table_key;
	else return "";
}
*/


void SQLDatabase::set_server_address(std::string address) { server_address = address; }
std::string SQLDatabase::get_server_address() { return server_address; }
void SQLDatabase::set_server_port(std::string port) { server_port = port; }
std::string SQLDatabase::get_server_port() { return server_port; }
void SQLDatabase::set_database_name(std::string db_name) { database_name = db_name; }
std::string SQLDatabase::get_database_name() { return database_name; }
void SQLDatabase::set_database_driver(std::string driver) { database_driver = driver; }
std::string SQLDatabase::get_database_driver() { return database_driver; }

void SQLDatabase::set_key(SQL_KEY key, std::string key_value)
{
	switch (key) {
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
	}
}

std::string SQLDatabase::get_key(SQL_KEY key)
{
	switch (key) {
	case FOODTYPE_TABLE_KEY: return foodtype_table_key;
	//case RECIPE_TABLE_KEY: return recipe_table_key;
	//case MEAL_TABLE_KEY: return meal_table_key;
	case NAME_KEY: return name_key;
	case CAL_KEY: return cal_key;
	case PROT_KEY: return prot_key;
	case CARB_KEY: return carb_key;
	case FAT_KEY: return fat_key;
	case SIZE_KEY: return size_key;
	default: return "";	//	maybe throw instead
	}
}
/*
DataBase::DTYPE SQL_database::contains(std::string name)
{
	QSqlQuery q_ft;
	q_ft.exec("SELECT NAME FROM " + QString::fromStdString(foodtype_table_key) 
		+ " WHERE NAME = " + QString::fromStdString(name) + ";");
	if (q_ft.size() > 0) return DataBase::FOODTYPE_T;

	QSqlQuery q_r;
	q_r.exec("SELECT NAME FROM " + QString::fromStdString(recipe_table_key)
		+ " WHERE NAME = " + QString::fromStdString(name) + ";");
	if (q_r.size() > 0) return DataBase::RECIPE_T;

	QSqlQuery q_m;
	q_m.exec("SELECT NAME FROM " + QString::fromStdString(meal_table_key)
		+ " WHERE NAME = " + QString::fromStdString(name) + ";");
	if (q_m.size() > 0) return DataBase::MEAL_T;

	return DataBase::EMPTY_T;
}
*/

std::vector<std::string> SQLDatabase::driver_list()
{
	QStringList qsl = QSqlDatabase::drivers();
	std::vector<std::string> ret;
	for (const QString& qs : qsl) ret.push_back(qs.toStdString());

	return ret;
}

bool SQLDatabase::contains(std::string name)
{
	QSqlQuery q;
	bool executed = q.exec("SELECT NAME FROM " + QString::fromStdString(foodtype_table_key)
		+ " WHERE NAME = " + "'" + QString::fromStdString(name) + "'" + ";");
	return executed && q.size() > 0;
}

bool SQLDatabase::contains(const FoodType& ft)
{
	return contains(ft.name());
}

FoodType SQLDatabase::get(std::string name)
{
	QSqlQuery q;
	bool executed = q.exec(QString::fromStdString("SELECT " + name_key + ", " + cal_key + ", " + prot_key + ", " + carb_key
		+ ", " + fat_key + ", " + size_key + " FROM " + foodtype_table_key
		+ " WHERE NAME = " + "'" + name + "'" + ";"));
	if (!executed || q.size() <= 0) throw new std::exception("Could not find name in SQL database");

	q.next();
	std::string nm = q.value(0).toString().toStdString();
	CAL_T cals = q.value(1).toDouble();
	WEIGHT_T prot = q.value(2).toDouble();
	WEIGHT_T carbs = q.value(3).toDouble();
	WEIGHT_T fats = q.value(4).toDouble();
	WEIGHT_T size = q.value(5).toDouble();

	return FoodType(nm, cals, prot, carbs, fats, size);
}

std::vector<FoodType> SQLDatabase::search(std::string name)
{

	QSqlQuery q;
	bool executed = q.exec(QString::fromStdString("SELECT " + name_key + ", " + cal_key + ", " + prot_key + ", " + carb_key
		+ ", " + fat_key + ", " + size_key + " FROM " + foodtype_table_key
		+ " WHERE NAME LIKE " + "'%" + name + "%'" + ";"));
	if (!executed || q.size() < 0) return {};//throw new std::exception("Problem searching for name in SQL database");

	std::vector<FoodType> ret;
	while (q.next()) {
		std::string name = q.value(0).toString().toStdString();
		CAL_T cals = q.value(1).toDouble();
		WEIGHT_T prot = q.value(2).toDouble();
		WEIGHT_T carbs = q.value(3).toDouble();
		WEIGHT_T fats = q.value(4).toDouble();
		WEIGHT_T size = q.value(5).toDouble();

		ret.push_back(FoodType(name, cals, prot, carbs, fats, size));
	}

	return ret;
}

std::vector<FoodType> SQLDatabase::all_foodtypes()
{
	QSqlQuery q;
	bool executed = q.exec(QString::fromStdString("SELECT " + name_key + ", " + cal_key + ", " + prot_key + ", " + carb_key + ", " + fat_key + ", " + size_key
		+ " FROM " + foodtype_table_key + ";"));
	if (!executed || q.size() <= 0) throw new std::exception("Could not find name in SQL database");

	std::vector<FoodType> ret;
	while (q.next()) {
		std::string name = q.value(0).toString().toStdString();
		CAL_T cals = q.value(1).toDouble();
		WEIGHT_T prot = q.value(2).toDouble();
		WEIGHT_T carbs = q.value(3).toDouble();
		WEIGHT_T fats = q.value(4).toDouble();
		WEIGHT_T size = q.value(5).toDouble();

		ret.push_back(FoodType(name, cals, prot, carbs, fats, size));
	}

	return ret;
}

bool SQLDatabase::add(const FoodType& ft)
{
	if (contains(ft)) return false;

	QSqlQuery q;
	bool executed = q.exec(QString::fromStdString("INSERT INTO " + foodtype_table_key
		+ " (" + name_key + ", " 
		+ cal_key + ", " 
		+ prot_key + ", " 
		+ carb_key + ", " 
		+ fat_key + ", " 
		+ size_key + ")" 
		+ " VALUES (" 
		+ "'"+ ft.name() + "'" + ", " 
		+ std::to_string(ft.get_cals()) + ", " 
		+ std::to_string(ft.get_prots()) + ", " 
		+ std::to_string(ft.get_carbs()) + ", " 
		+ std::to_string(ft.get_fats()) + ", " 
		+ std::to_string(ft.get_size()) + ")" +
		";"));
	
	return executed;
}

int SQLDatabase::add(const std::vector<FoodType>& vec)
{
	int sum = 0;
	for (const FoodType& ft : vec) {
		if(add(ft)) sum++;
	}
	return sum;
}

bool SQLDatabase::update(const FoodType& ft)
{
	if (!contains(ft)) return add(ft);
	else {
		QSqlQuery q;
		bool executed = q.exec(QString::fromStdString("UPDATE " + foodtype_table_key
			+ " SET " 
			+ cal_key + " = " + std::to_string(ft.get_cals()) + ", " 
			+ prot_key  + " = " + std::to_string(ft.get_cals()) + ", "
			+ carb_key + " = " + std::to_string(ft.get_carbs()) + ", "
			+ fat_key + " = " + std::to_string(ft.get_fats()) + ", "
			+ size_key + " = " + std::to_string(ft.get_size()) 
			+ " WHERE " + name_key + " = " + "'" + ft.name() + "'"
			+ ";"));

		return executed;
	}

}

int SQLDatabase::update(const std::vector<FoodType>& vec)
{
	int sum = 0;
	for (const FoodType& ft : vec) {
		if (update(ft)) sum++;
	}
	return sum;
}

bool SQLDatabase::remove(std::string name)
{
	QSqlQuery q;
	bool executed = q.exec(QString::fromStdString(
		"DELETE FROM " + foodtype_table_key 
		+ " WHERE " + name_key + " = " + "'" + name + "'" + ";"
	));

	return executed;
}
