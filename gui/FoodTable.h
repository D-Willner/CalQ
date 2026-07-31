#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qtablewidget.h"
#include "database/DataBase.h"
#include "database/Food.h"

class FoodTable :
    public QTableWidget
{
    Q_OBJECT
private:
    bool row_is_free(int row);

    int min_rows = 1;


public:
    enum FTTYPE{NO_FACTOR, FACTOR};

    explicit FoodTable(int rows, FTTYPE = NO_FACTOR, QWidget* parent = nullptr);

    void set_editable(bool can_edit);

    bool set_factor(double factor, int row);

    bool set_food(const Food& f, int row);

    bool set_food_no_amount(const Food& f, int row);

    bool has_food(int row);

    Food read_food(int row);

    bool has_factor(int row);

    double read_factor(int row);

    void insert_row(int row);

    void add_row();

    void clear_row(int row);

    void set_min_rows(int rows);

    int get_min_rows();

    void clear_remove_row(int row);

    void moveRowsUp(int r = 0);

    //  void removeRow(int) from parent

public slots:

    void add_meal(const Meal& m);

    void add_food(const Food& f);

    void insert_food(const Food& f, int row);

    void clear_table();

    void process_double_click(QTableWidgetItem* item);

signals:

    void food_removed(const Food& f);

    void food_double_clicked(const Food& f, int row);
};

