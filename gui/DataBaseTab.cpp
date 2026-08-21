#include "DataBaseTab.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "RecipeDialog.h"
#include "ToolTipLabel.h"
#include <QMessageBox>
#include <QThread>
#include "AIConfigDialog.h"
#include "SQLConfigDialog.h"
#include "StyleSheets.h"

DataBaseTab::DataBaseTab(DataBase& db, SQLDatabase& sql_db, Settings& s, QWidget* parent) 
	: QWidget(parent), database(db), sql_database(sql_db), settings(s)
{
	client = new Client(settings.get_ai_settings(), this);

	food_type_table = new FoodTable(5);
	food_type_table->set_name_editable(false);
	recipe_table = new FoodTable(5);
	recipe_table->set_editable(false);
	meal_table = new FoodTable(5);
	meal_table->set_editable(false);

	exercise_table = new ExerciseTable(5);
	exercise_table->set_name_editable(false);

	ToolTipLabel* SQL_label = new ToolTipLabel("Search for food types in the SQL database.\nManage and configure the database.", "SQL Database:");
	SQL_label->setStyleSheet(TTLABEL_STYLE);
	SQL_label->set_margin(5);
	SQL_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	SQL_search_name = new QLineEdit;
	SQL_search_name->setMinimumWidth(200);
	SQL_search_name->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	SQL_search_btn = new QPushButton("Search");
	SQL_search_btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	SQL_search_results = new QListWidget;
	SQL_search_results->setMinimumWidth(400);

	SQL_search_selection = new FoodTable(1);
	SQL_search_selection->set_editable(false);

	QPushButton* SQL_add_btn = new QPushButton("Add");
	QPushButton* SQL_configure_btn = new QPushButton("Configure");
	QPushButton* SQL_upload_btn = new QPushButton("Upload");
	QPushButton* SQL_download_btn = new QPushButton("Download");
	SQL_connect_btn = new QPushButton("Connect"); 

	update_sql_connect_btn();

	QVBoxLayout* layout_SQL = new QVBoxLayout;

	QHBoxLayout* layout_SQL_search = new QHBoxLayout;
	layout_SQL_search->addWidget(SQL_search_name,0, Qt::AlignLeft);
	layout_SQL_search->addWidget(SQL_search_btn,1,Qt::AlignLeft);

	QHBoxLayout* layout_SQL_bottom = new QHBoxLayout;
	layout_SQL_bottom->addWidget(SQL_add_btn, 1, Qt::AlignLeft);
	layout_SQL_bottom->addWidget(SQL_upload_btn, 0, Qt::AlignRight);
	layout_SQL_bottom->addWidget(SQL_download_btn, 0, Qt::AlignRight);
	layout_SQL_bottom->addWidget(SQL_connect_btn, 0, Qt::AlignRight);
	layout_SQL_bottom->addWidget(SQL_configure_btn, 0, Qt::AlignRight);

	layout_SQL->addWidget(SQL_label);
	layout_SQL->addLayout(layout_SQL_search);
	layout_SQL->addWidget(SQL_search_results,0,Qt::AlignLeft);
	layout_SQL->addWidget(SQL_search_selection,0,Qt::AlignLeft);
	layout_SQL->addLayout(layout_SQL_bottom);

	AI_search_name = new QLineEdit;
	AI_search_name->setMinimumWidth(200);

	AI_search_result = new FoodTable(1);
	AI_search_result->set_editable(true);

	AI_model_list = new QComboBox;
	AI_model_list->setMinimumWidth(100);
	//AI_model_list->

	ToolTipLabel* AI_label = new ToolTipLabel("Search for foods using AI.\nConfigure and manage the AI model.", "AI Search:");
	AI_label->setStyleSheet(TTLABEL_STYLE);
	AI_label->set_margin(5);
	AI_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	QPushButton* AI_search_btn = new QPushButton("Ask");
	QPushButton* AI_add_btn = new QPushButton("Add");
	QPushButton* AI_model_update_btn = new QPushButton("Update");
	AI_start_server_btn = new QPushButton("Start server");
	QPushButton* AI_config_btn = new QPushButton("Configure");

	QHBoxLayout* layout_AI_search = new QHBoxLayout;
	layout_AI_search->addWidget(AI_search_name);
	layout_AI_search->addWidget(AI_search_btn, 1, Qt::AlignLeft);
	layout_AI_search->addWidget(new QLabel("Select model:"), 0, Qt::AlignRight);
	layout_AI_search->addWidget(AI_model_list, 0, Qt::AlignRight);
	layout_AI_search->addWidget(AI_model_update_btn, 0, Qt::AlignRight);

	QHBoxLayout* layout_AI_bottom= new QHBoxLayout;
	layout_AI_bottom->addWidget(AI_add_btn,1,Qt::AlignLeft);
	layout_AI_bottom->addWidget(AI_start_server_btn, 0, Qt::AlignRight);
	layout_AI_bottom->addWidget(AI_config_btn, 0, Qt::AlignRight);

	QVBoxLayout* layout_left = new QVBoxLayout;

	QFrame* food_type_frame = new QFrame;
	food_type_frame->setStyleSheet(FRAME_STYLE);
	QVBoxLayout* layout_food_type_frame = new QVBoxLayout(food_type_frame);
	ToolTipLabel* tt_food_type = new ToolTipLabel("Displays all the foods in the database.\n\
Modify them by changing the values.\n\
Remove them by pressing the \"-\" button.", "Foods:");
	tt_food_type->setStyleSheet(TTLABEL_STYLE);
	tt_food_type->set_margin(5);
	tt_food_type->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	layout_food_type_frame->addWidget(tt_food_type);
	layout_food_type_frame->addWidget(food_type_table, 1, Qt::AlignLeft | Qt::AlignTop);

	QFrame* recipe_frame = new QFrame;
	recipe_frame->setStyleSheet(FRAME_STYLE);
	QVBoxLayout* layout_recipe_frame = new QVBoxLayout(recipe_frame);
	ToolTipLabel* tt_recipe = new ToolTipLabel("Displays all the recipes in the database.\n\
Modify them by double clicking.\n\
Remove them by pressing the \"-\" button.", "Recipes:");
	tt_recipe->setStyleSheet(TTLABEL_STYLE);
	tt_recipe->set_margin(5);
	tt_recipe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	layout_recipe_frame->addWidget(tt_recipe);
	layout_recipe_frame->addWidget(recipe_table, 1, Qt::AlignLeft | Qt::AlignTop);

	QFrame* meal_frame = new QFrame;
	meal_frame->setStyleSheet(FRAME_STYLE);
	QVBoxLayout* layout_meal_frame = new QVBoxLayout(meal_frame);
	ToolTipLabel* tt_meal = new ToolTipLabel("Displays all the meals in the database.\n\
Remove them by pressing the \"-\" button.", "Meals:");
	tt_meal->setStyleSheet(TTLABEL_STYLE);
	tt_meal->set_margin(5);
	tt_meal->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	layout_meal_frame->addWidget(tt_meal);
	layout_meal_frame->addWidget(meal_table, 1, Qt::AlignLeft | Qt::AlignTop);

	layout_left->addWidget(food_type_frame);
	layout_left->addWidget(recipe_frame);
	layout_left->addWidget(meal_frame);

	QVBoxLayout* layout_right = new QVBoxLayout;

	QFrame* exercise_frame = new QFrame;
	exercise_frame->setStyleSheet(FRAME_STYLE);
	QVBoxLayout* layout_exercise_frame = new QVBoxLayout(exercise_frame);
	ToolTipLabel* tt_exercise = new ToolTipLabel("Displays all the exercises in the database.\n\
Modify them by changing the values.\n\
Remove them by pressing the \"-\" button.", "Exercises:");
	tt_exercise->setStyleSheet(TTLABEL_STYLE);
	tt_exercise->set_margin(5);
	tt_exercise->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	layout_exercise_frame->addWidget(tt_exercise);
	layout_exercise_frame->addWidget(exercise_table, 1, Qt::AlignLeft | Qt::AlignTop);

	QFrame* SQL_frame = new QFrame;
	SQL_frame->setStyleSheet(FRAME_STYLE);
	SQL_frame->setLayout(layout_SQL);

	QFrame* AI_frame = new QFrame;
	AI_frame->setStyleSheet(FRAME_STYLE);
	QVBoxLayout* AI_layout = new QVBoxLayout(AI_frame);
	AI_layout->addWidget(AI_label);
	AI_layout->addLayout(layout_AI_search);
	AI_layout->addWidget(AI_search_result);
	AI_layout->addLayout(layout_AI_bottom);

	layout_right->addWidget(exercise_frame);
	layout_right->addWidget(SQL_frame, 1);
	layout_right->addWidget(AI_frame);

	QHBoxLayout* layout = new QHBoxLayout;

	layout->addLayout(layout_left,1);
	layout->addSpacing(10);
	layout->addLayout(layout_right);

	setLayout(layout);

	QObject::connect(food_type_table, &FoodTable::food_removed,
		this, [=](const Food& f) {database.remove_foodtype(f.name());});
	QObject::connect(recipe_table, &FoodTable::food_removed,
		this, [=](const Food& f) {database.remove_recipe(f.name());});
	QObject::connect(meal_table, &FoodTable::food_removed,
		this, [=](const Food& f) {database.remove_meal(f.name());});
	QObject::connect(exercise_table, &ExerciseTable::exercise_removed,
		this, [=](const Exercise& e) {database.remove_exercisetype(e.name());});

	QObject::connect(food_type_table, &FoodTable::cellChanged, this, &DataBaseTab::handle_ft_change);
	QObject::connect(recipe_table, &FoodTable::cellDoubleClicked, this, &DataBaseTab::handle_rec_change);

	//	SQL database connections
	QObject::connect(SQL_search_btn, &QPushButton::clicked,
		this, [&]() {
			std::vector<FoodType> results = sql_database.search(SQL_search_name->text().toStdString());
			SQL_search_results->clear();
			for (const FoodType& ft : results) {
				SQL_search_results->addItem(QString::fromStdString(ft.name()) + " (" + QString::number(ft.get_cals()) + " cal, " + QString::number(ft.get_prots()) + " prot, " + QString::number(ft.get_carbs()) + " carbs, " + QString::number(ft.get_fats()) + " fats)");
			}
	});
	
	QObject::connect(SQL_search_results, &QListWidget::itemClicked,
		this, [&](QListWidgetItem* item) {
			std::string name = item->text().toStdString();
			name = name.substr(0, name.find(" ("));
			FoodType ft = sql_database.get(name);
			SQL_search_selection->clear_table();
			SQL_search_selection->add_food(Food(ft,ft.get_size()));
	});

	QObject::connect(SQL_add_btn, &QPushButton::clicked,
		this, [&]() {
			if (SQL_search_selection->has_food(0)) {
				database.overwrite(SQL_search_selection->read_food(0).food_type());
				SQL_search_selection->clear_table();

				food_type_table->clear_table();	//	Could be more efficient 
				load_food_types();
			}
	});

	QObject::connect(SQL_download_btn, &QPushButton::clicked,
		this, [&]() {
			for (const FoodType& ft : sql_database.all_foodtypes()) {
				database.overwrite(ft);
			}
	});

	QObject::connect(SQL_upload_btn, &QPushButton::clicked,
		this, [&]() {
			sql_database.update(database.foodtype_name_contains(""));
	});

	QObject::connect(SQL_connect_btn, &QPushButton::clicked,
		this, [&]() {
			if (sql_database.is_connected()) {
				sql_database.disconnect();
			}
			else {
				sql_database.connect();
			}
			update_sql_connect_btn();
	});

	QObject::connect(SQL_configure_btn, &QPushButton::clicked, this, [&]() {
		SQLConfigDialog* dialog = new SQLConfigDialog(sql_database, settings, this);
		dialog->open();
	});

	//	AI database connections
	QObject::connect(AI_start_server_btn, &QPushButton::clicked, 
		this, [&]() {
			bool to_start = AI_start_server_btn->text() == "Start server";
			LMS_server_manage(to_start ? START_SERVER : STOP_SERVER);
			//AI_start_server_btn->setText(to_start ? "Stop" : "Start");	
			update_AI_start_btn();	//	only works because server_manage hangs until complete
	});

	QObject::connect(AI_model_update_btn, &QPushButton::clicked, client, &Client::request_models);
	QObject::connect(client, &Client::models, this, &DataBaseTab::update_models);

	QObject::connect(AI_search_btn, &QPushButton::clicked, 
		this, [&](){
			client->request_food_data(AI_search_name->text().toStdString());
	});
	QObject::connect(client, &Client::food_data, this, &DataBaseTab::handle_food_arrival);

	QObject::connect(AI_model_list, &QComboBox::currentTextChanged,
		this, [&](const QString& qs) {
			client->set_model_name(last_models[qs.toStdString()]);
			auto ai_settings = settings.get_ai_settings();
			ai_settings.set_key(AISettings::LAST_MODEL_USED, last_models[qs.toStdString()]);
			settings.set_ai_settings(ai_settings);
	});

	QObject::connect(AI_config_btn, &QPushButton::clicked, this, [&]() {
		AIConfigDialog* dialog = new AIConfigDialog(settings, *client, this);
		dialog->open();
	});

	QObject::connect(AI_add_btn, &QPushButton::clicked, this, &DataBaseTab::AI_add_food);
	QObject::connect(AI_add_btn, &QPushButton::clicked, AI_search_name, &QLineEdit::clear);

	QObject::connect(this, &DataBaseTab::server_running,
		this, [&](bool is_running) {
			AI_start_server_btn->setText(is_running ? "Stop server" : "Start server");
	});
	update_AI_start_btn();

	setFocusPolicy(Qt::ClickFocus);
}

void DataBaseTab::update_sql_connect_btn() {
	if (sql_database.is_connected()) {
		SQL_connect_btn->setText("Disconnect");
	}
	else {
		SQL_connect_btn->setText("Connect");
	}
};

void DataBaseTab::fetch_models()
{
	client->request_models();
}

void DataBaseTab::handle_ft_change(int row, int col)
{
	if (row >= food_type_table->rowCount() || col >= 6) return;

	if(col > 0 && food_type_table->has_food(row)){
		Food f = food_type_table->read_food(row);
		if(f.name() != "") database.overwrite(f.food_type());
	}
}

void DataBaseTab::handle_rec_change(int row, int col)
{
	if (row >= recipe_table->rowCount() || col >= 6) return;

	auto name_item = recipe_table->item(row, 0);
	if (!name_item) return;

	std::string name = name_item->text().toStdString();
	auto res = database.contains(name);
	if (res != DataBase::RECIPE_T) return;
	
	Recipe r = database.get_recipe(name);
	RecipeDialog* rd = new RecipeDialog(r,database,this);
	QObject::connect(rd, &RecipeDialog::found,
		this, [this](const Recipe& r) {
			database.overwrite(r);
			for (const Food& f : r.get_ingredients()) {
				database.add(f.food_type());
			}
			recipe_table->clear_table();
			load_recipes();	//	somewhat inefficient
		}
	);
	rd->show();
}

void DataBaseTab::AI_add_food()
{
	if (AI_search_result->has_food(0)) {
		database.overwrite(AI_search_result->read_food(0).food_type());
		AI_search_result->clear_table();

		food_type_table->clear_table();	//	Could be more efficient 
		load_food_types();
	}
}

bool DataBaseTab::LMS_server_manage(SERVER_OP op)
{
	QProcess* qp = new QProcess;
	qp->setProgram("lms");
	qp->setArguments({ "server", op == START_SERVER ? "start" : "stop"});
	qp->start();
	qp->waitForFinished();	//	maybe better this way

	qp->deleteLater();

	return true;
}

//	pointless, kind of
void DataBaseTab::update_AI_start_btn()
{
	LMS_server_running_dispatch();
}

bool DataBaseTab::LMS_server_running()
{
	QProcess* qp = new QProcess;
	qp->setProgram("lms");
	qp->setArguments({ "server", "status" });
	qp->start();
	qp->waitForFinished();
	
	auto ba = qp->readAllStandardError();
	QString str = QString::fromUtf8(ba);

	qp->deleteLater();

	if (str.contains("The server is running on")) {
		emit server_running(true);
		return true;
	}
	else if (str.contains("The server is not running")) {
		emit server_running(false);
	}
	return false;
}

void DataBaseTab::LMS_server_running_dispatch()
{
	QProcess* qp = new QProcess();
	QObject::connect(qp, &QProcess::finished,
		this, [=](int res, QProcess::ExitStatus status) {LMS_server_running_result(qp);});
	qp->setProgram("lms");
	qp->setArguments({ "server", "status" });
	qp->start();
}

void DataBaseTab::LMS_server_running_result(QProcess* qp)
{
	auto ba = qp->readAllStandardError();
	QString str = QString::fromUtf8(ba);

	qp->deleteLater();
	//QMessageBox* mb = new QMessageBox(QMessageBox::NoIcon, "Result", str);
	//mb->show();

	if (str.contains("The server is not running")) emit server_running(false);
	else if (str.contains("The server is running on")) emit server_running(true);
}

void DataBaseTab::update_models(const std::vector<std::pair<std::string, std::string>>& models)
{
	AI_model_list->clear();
	last_models.clear();

	std::string last_used_model = settings.get_ai_settings().get_key(AISettings::LAST_MODEL_USED);
	std::string last_model_display_name = "";
	for (const auto& p : models) {
		AI_model_list->addItem(QString::fromStdString(p.first));
		last_models[p.first] = p.second;
		if(p.second == last_used_model) {
			last_model_display_name = p.first;
		}
	}
	if (last_model_display_name != "") {
		AI_model_list->setCurrentText(QString::fromStdString(last_model_display_name));
	}
}

void DataBaseTab::handle_food_arrival(const FoodType& ft)
{
	AI_search_result->clear_table();
	AI_search_result->add_food(Food(ft, ft.get_size()));
}

void DataBaseTab::load_tables()
{
	load_food_types();
	load_recipes();
	load_meals();
	load_exercises();
}

void DataBaseTab::load_food_types()
{
	std::vector<FoodType> vft = database.foodtype_name_contains("");
	for (const auto& ft : vft) {
		food_type_table->add_food(Food(ft,ft.get_size()));
	}
	for (int row = 0; row < food_type_table->rowCount(); row++) {
		auto name_item = food_type_table->item(row, 0);
		if(name_item != nullptr) name_item->setFlags(name_item->flags() & ~Qt::ItemIsEditable);
	}
}

void DataBaseTab::load_recipes()
{
	std::vector<Recipe> vr = database.recipe_name_contains("");
	for (const auto& r : vr) {
		recipe_table->add_food(r.as_food());
	}
}

void DataBaseTab::load_meals()
{
	std::vector<Meal> vm = database.meal_name_contains("");
	for (const auto& m : vm) {
		meal_table->add_meal(m);
	}
}

void DataBaseTab::load_exercises()
{
	std::vector<ExerciseType> ve = database.exercisetype_name_contains("");
	for (const auto& e : ve) {
		exercise_table->addExercise(Exercise(e, 60));
	}
}

void DataBaseTab::clear_tables()
{
	food_type_table->clear_table();
	recipe_table->clear_table();
	meal_table->clear_table();
	exercise_table->clear_table();
}


void DataBaseTab::update_tables()
{
	clear_tables();
	load_tables();
}