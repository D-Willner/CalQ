#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <string>
#include <vector>
#include "database/FoodType.h"
#include "database/database.h"
#include "SQLSettings.h"

class SQLDatabase {

private:
	QSqlDatabase qsql_database;

	std::string server_address;
	std::string server_port;
	std::string database_name;
	std::string database_driver;

	std::string foodtype_table_key = "FOODTYPES";
	//std::string recipe_table_key = "RECIPES";
	//std::string meal_table_key = "MEALS";

	std::string name_key = "NAME";
	std::string cal_key = "CALS";
	std::string prot_key = "PROT";
	std::string carb_key = "CARBS";
	std::string fat_key = "FATS";
	std::string size_key = "SIZE";

	/* 
	std::string exercisetype_table_key;
	std::string exercisename_key;
	std::string exercisecal_key;
	*/
	//std::string exercisedur_key;

protected:
	/*	in future recipes, meals and exercisetypes could be saved to database
	template<typename T>
	std::string key_from_type(const T&);

	template<typename T>
	bool contains(const T&);

	template<typename T>
	T get(std::string name);

	template<typename T>
	std::vector<T> all_T();

	template<typename T>
	bool add(const T&);

	template<typename T>
	bool add(const std::vector<T>&);

	template<typename T>
	bool update(const T&);

	template<typename T>
	bool update(const std::vector<T>&);

	template<typename T>
	bool remove(const T&);
	*/

public:
	enum SQL_KEY{
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

	static std::vector<std::string> driver_list();

	SQLDatabase(const SQLSettings& settings);
	SQLDatabase(std::string db_name = "", std::string db_driver = "QPSQL", std::string address = "127.0.0.1", std::string port = "5432");
	SQLDatabase(const SQLDatabase&);

	bool connect(std::string db_name , std::string address);
	bool connect(std::string db_name);
	bool connect();

	bool is_connected();
	bool initialize_db();
	bool disconnect();

	bool contains(std::string name);
	bool contains(const FoodType&);

	FoodType get(std::string name);
	std::vector<FoodType> search(std::string name);
	std::vector<FoodType> all_foodtypes();

	bool add(const FoodType&);
	int add(const std::vector<FoodType>&);

	bool update(const FoodType&);
	int update(const std::vector<FoodType>&);

	bool remove(std::string name);

	void set_settings(const SQLSettings& settings);
	SQLSettings get_settings() const;


	//	config
	void set_server_address(std::string address);
	std::string get_server_address();
	void set_server_port(std::string port);
	std::string get_server_port();
	void set_database_name(std::string db_name);
	std::string get_database_name();
	void set_database_driver(std::string driver);
	std::string get_database_driver();

	void set_key(SQL_KEY, std::string key_value);
	std::string get_key(SQL_KEY);
};

