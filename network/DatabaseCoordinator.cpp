#include "DatabaseCoordinator.h"

DatabaseCoordinator::DatabaseCoordinator(DataBase& db, SQLDatabase& sql_db) : database(db), sql_database(sql_db) {}


void DatabaseCoordinator::upload_to_SQL()
{
	sql_database.update(database.foodtype_name_contains(""));
}

void DatabaseCoordinator::download_from_SQL()
{
	for (const FoodType& ft : sql_database.all_foodtypes()) {
		database.overwrite(ft);
	}
}