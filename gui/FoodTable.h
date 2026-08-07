#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qtablewidget.h"
#include "database/DataBase.h"
#include "database/Food.h"

class FoodTable :
    public QTableWidget
{
    Q_OBJECT
private:
    int min_rows = 1;

    bool name_editable = true;

    bool amount_adjust = false;

    bool row_is_free(int row);

    std::vector<WEIGHT_T> prev_weights;

    constexpr static int NAME_COL_SIZE = 200;
    constexpr static int VALUE_COL_SIZE = 60;

public slots:
    bool adjust_row(int row);   //  afteconstexprr a weight change in the row

public:
    enum FTTYPE{NO_FACTOR, FACTOR};

    explicit FoodTable(int rows, FTTYPE = NO_FACTOR, QWidget* parent = nullptr);

    void set_amount_adjust(bool does_adjust);
    bool get_amount_adjust();

    void set_editable(bool can_edit);
    void set_name_editable(bool can_edit);

    bool set_factor(double factor, int row);

    bool set_food(const Food& f, int row);

    bool set_food_no_amount(const Food& f, int row);

    bool has_food(int row);

    Food read_food(int row);

    std::vector<Food> read_ingredients();

    bool has_factor(int row);

    double read_factor(int row);

    void insert_row(int row);

    void add_row();

    void clear_row(int row);

    void set_min_rows(int rows);

    int get_min_rows();

    void clear_remove_row(int row);
    void clear_remove_emit_row(int row);

    void move_rows_up(int r = 0);

    //  void removeRow(int) from parent

public slots:

    void add_recipe(const Recipe& r);

    void add_meal(const Meal& m);

    void add_food(const Food& f);

    void insert_food(const Food& f, int row);

    void clear_table();

    void process_double_click(QTableWidgetItem* item);

signals:

    void food_removed(const Food& f);

    void food_double_clicked(const Food& f, int row);

    void adjusted(int row, std::string name);
};

