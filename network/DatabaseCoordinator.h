#pragma once
#include "database/database.h"
#include "SQLDatabase.h"

class DatabaseCoordinator
{
private:
	DataBase& database;
	SQLDatabase& sql_database;

public:
	DatabaseCoordinator(DataBase& db, SQLDatabase& sql_db);

	void upload_to_SQL();
	void download_from_SQL();
};

