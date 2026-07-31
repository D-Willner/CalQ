#include "FoodTable.h"
#include <qheaderview.h>
#include <QPushButton>

FoodTable::FoodTable(int rows, FTTYPE type, QWidget* parent) : QTableWidget(parent)
{
	//this->setRowCount(rows);
	this->setColumnCount(type == NO_FACTOR ? 7 : 8);

    if (type == NO_FACTOR) {
        setHorizontalHeaderLabels({ "Name", "Amount", "Cals", "Prot", "Carbs", "Fats", "" });
        setMinimumWidth(475);   //  magic number that just works
    }
    else if (type == FACTOR) {
        setHorizontalHeaderLabels({ "Name", "Amount", "Cals", "Prot", "Carbs", "Fats", "Factor", "" });
        setMinimumWidth(535);
    }
    
    this->setColumnWidth(1, 60);
    this->setColumnWidth(2, 60);
    this->setColumnWidth(3, 60);
    this->setColumnWidth(4, 60);
    this->setColumnWidth(5, 60);
    this->setColumnWidth(6, 60);
    if (type == FACTOR) this->setColumnWidth(7, 60);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->verticalHeader()->hide();

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < rows; i++) add_row();

    QObject::connect(this, &FoodTable::itemDoubleClicked, this, &FoodTable::process_double_click);
}

void FoodTable::set_editable(bool can_edit)
{
    if (can_edit) {
        this->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed);
        this->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else {
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->setSelectionMode(QAbstractItemView::NoSelection);
    }
}

bool FoodTable::set_factor(double factor, int row)
{
    if (row >= rowCount() || row < 0 || columnCount() != 8) return false;

    setItem(row, 6, new QTableWidgetItem(QString::number(factor)));
    return true;
}

bool FoodTable::set_food(const Food& f, int row) {
    if (this->rowCount() <= row) return false;

    set_food_no_amount(f, row);

    setItem(row, 1, new QTableWidgetItem(QString::number(f.grams())));

    return true;
}

bool FoodTable::set_food_no_amount(const Food& f, int row)
{
    if (this->rowCount() <= row) return false;

    setItem(row, 0, new QTableWidgetItem(QString::fromStdString(f.name())));

    setItem(row, 2, new QTableWidgetItem(QString::number(f.calories())));
    setItem(row, 3, new QTableWidgetItem(QString::number(f.protein())));
    setItem(row, 4, new QTableWidgetItem(QString::number(f.carbs())));
    setItem(row, 5, new QTableWidgetItem(QString::number(f.fats())));

    return true;
}


bool FoodTable::row_is_free(int row)
{
    bool name_free      = (item(row, 0) == nullptr || item(row, 0)->text() == "");
    bool amount_free    = (item(row, 1) == nullptr || item(row, 1)->text() == "");
    bool cals_free      = (item(row, 2) == nullptr || item(row, 2)->text() == "");
    bool prot_free      = (item(row, 3) == nullptr || item(row, 3)->text() == "");
    bool carbs_free     = (item(row, 4) == nullptr || item(row, 4)->text() == "");
    bool fats_free      = (item(row, 5) == nullptr || item(row, 5)->text() == "");

    return name_free && amount_free && cals_free && prot_free && carbs_free && fats_free;
}

void FoodTable::add_food(const Food& f)
{
    for (int i = 0; i < rowCount(); i++) {
        if (row_is_free(i)) {
            set_food(f, i);
            return;
        }
    }

    this->add_row();
    set_food(f, rowCount() - 1);
}

void FoodTable::add_meal(const Meal& m)
{
    for (int i = 0; i < rowCount(); i++) {
        if (row_is_free(i)) {
            set_food(m, i);
            set_factor(m.percentage_left(), i);
            return;
        }
    }

    this->add_row();
    set_food(m, rowCount() - 1);
    set_factor(m.percentage_left(), rowCount() - 1);
}


void FoodTable::insert_food(const Food& f, int row)
{
    insert_row(row);
    set_food(f, row);
}


bool FoodTable::has_food(int row)
{
    if (row_is_free(row)) return false;

    QTableWidgetItem* cur;
    cur = this->item(row, 0);
    std::string name = cur == nullptr ? "" : cur->text().toStdString();

    bool ret = true, ok = true;
    cur = this->item(row, 1);
    WEIGHT_T amount = cur == nullptr ? 0 : cur->text().toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 2);
    CAL_T cals = cur == nullptr ? 0 : cur->text().toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 3);
    WEIGHT_T prot = cur == nullptr ? 0 : cur->text().toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 4);
    WEIGHT_T carbs = cur == nullptr ? 0 : cur->text().toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 5);
    WEIGHT_T fats = cur == nullptr ? 0 : cur->text().toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 6);
    double factor = cur == nullptr ? 0 : cur->text().toDouble(&ret);
    ok &= ret;

    return ok;
}

Food FoodTable::read_food(int row)
{
    QTableWidgetItem* cur;
    cur = this->item(row, 0);
    std::string name = cur == nullptr ? "" : cur->text().toStdString();
    

    bool ret = true, ok = true;
    cur = this->item(row, 1);
    WEIGHT_T amount = cur == nullptr ? 0 : cur->data(Qt::EditRole).toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 2);
    CAL_T cals = cur == nullptr ? 0 : cur->data(Qt::EditRole).toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 3);
    WEIGHT_T prot = cur == nullptr ? 0 : cur->data(Qt::EditRole).toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 4);
    WEIGHT_T carbs = cur == nullptr ? 0 : cur->data(Qt::EditRole).toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 5);
    WEIGHT_T fats = cur == nullptr ? 0 : cur->data(Qt::EditRole).toDouble(&ret);
    ok &= ret;

    cur = this->item(row, 6);
    double factor = cur == nullptr ? 0 : cur->data(Qt::EditRole).toDouble(&ret);
    ok &= ret;

    if (!ok) {
        throw new std::exception("Could not convert table data into food data");
    }
    if (amount == 0) amount = 100;
    FoodType ft(name, cals * 100 / amount, prot * 100 / amount, carbs * 100 / amount, fats * 100 / amount, amount);

    return Food(ft, amount);
}

void FoodTable::insert_row(int row)
{
    QPushButton* btn = new QPushButton("-");
    insertRow(row);
    setCellWidget(row, columnCount()-1, btn);

    QObject::connect(btn, &QPushButton::clicked, this, [=]() {this->clear_remove_row(currentRow());});
}

void FoodTable::add_row()
{
    insert_row(rowCount());
}

void FoodTable::clear_row(int row)
{
    if (row >= rowCount()) return;

    for (int i = 0; i < columnCount(); i++) {
        setItem(row, i, nullptr);
    }
}

void FoodTable::moveRowsUp(int r)
{
    for (int i = r; i < rowCount(); i++) {
        if (!row_is_free(i)) continue;

        for (int j = i + 1; j < rowCount(); j++) {
            if (has_food(j)) {
                Food f = read_food(j);
                set_food(f, i);
                clear_row(j);
                break;
            }
        }
    }
}

void FoodTable::set_min_rows(int rows) { min_rows = rows; }

int FoodTable::get_min_rows() { return min_rows; }

void FoodTable::clear_remove_row(int row)
{
    if (has_food(row)) {
        Food f = read_food(row);
        emit food_removed(f);
    }

    if (rowCount() > min_rows) removeRow(row);
    else {
        clear_row(row);
        moveRowsUp(row);
    }
}


void FoodTable::clear_table()
{
    int i = 0;
    while (i < rowCount()) {
        clear_remove_row(i);
        if (row_is_free(i)) i++;
    }
}

void FoodTable::process_double_click(QTableWidgetItem* item)
{
    int row = item->row();
    if (has_food(row)) {
        Food f = read_food(row);
        emit food_double_clicked(f, row);
    }
}

bool FoodTable::has_factor(int row)
{
    if (row < 0 || row >= rowCount() || columnCount() < 8) return false;

    if (item(row, 6) == nullptr || item(row, 6)->text() == "") return false;
    auto f = item(row, 6)->data(Qt::EditRole);
    bool res;
    f.toDouble(&res);

    return res;
}

double FoodTable::read_factor(int row)
{
    auto f = item(row, 6)->data(Qt::EditRole);
    bool res;
    double factor = f.toDouble(&res);

    return factor;
}





