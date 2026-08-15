#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include "FoodTable.h"
#include "ExerciseTable.h"
#include "database/DataBase.h"
#include "AIConfigDialog.h"
#include "network/client.h"
#include <QComboBox>
#include <QProcess>

class DataBaseTab :
    public QWidget
{
    Q_OBJECT

private:
    DataBase& database;

    //  key is display_name, stored data is model key for API
    std::map<std::string, std::string> last_models;

    Client* client;
    //QProcess* qp;

    FoodTable* food_type_table;
    FoodTable* recipe_table;
    FoodTable* meal_table;
    ExerciseTable* exercise_table;

    QLineEdit* AI_search_name;
    FoodTable* AI_search_result;
    QComboBox* AI_model_list;
    QPushButton* AI_start_server_btn;

    void load_tables();
    void load_food_types();
    void load_recipes();
    void load_meals();
    void load_exercises();

    void clear_tables();


    enum SERVER_OP{START_SERVER, STOP_SERVER};

public:
    explicit DataBaseTab(DataBase& db, QWidget* parent = nullptr);

    void update_AI_start_btn();
    bool LMS_server_running();

signals:
    void server_running(bool is_running);

private slots:
    void handle_ft_change(int row, int col);
    void handle_rec_change(int row, int col);

    void LMS_server_running_result(QProcess*);

    void update_models(const std::vector<std::pair<std::string, std::string>>&);

    void handle_food_arrival(const FoodType&);

    void AI_add_food();

public slots:
    void update_tables();
    bool LMS_server_manage(SERVER_OP op);
    void fetch_models();
    void LMS_server_running_dispatch();
};

