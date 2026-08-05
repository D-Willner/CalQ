 #include "ExerciseTable.h"
#include <QHeaderView>
#include <QPushButton>

ExerciseTable::ExerciseTable(int rows, QWidget* parent) : QTableWidget(parent)
{
    setColumnCount(4);
    setHorizontalHeaderLabels({ "Name", "Duration", "Calories", ""});

    setColumnWidth(1, 60);
    setColumnWidth(2, 60);
    setColumnWidth(3, 60);
    setFixedWidth(300);
    
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    verticalHeader()->hide();
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < rows; i++) add_row();

    set_editable(false);
    setFixedHeight(rowHeight(0) * (rows + 1));
    set_minimum_rows(rows);
}

void ExerciseTable::insert_row(int row)
{
    QTableWidget::insertRow(row);
    QPushButton* btn = new QPushButton("-");
    setCellWidget(row, columnCount() - 1, btn);

    QObject::connect(btn, &QPushButton::clicked, this, [=]() {clear_remove_emit_row(currentRow());});
}

void ExerciseTable::add_row()
{
    insert_row(rowCount());
}

void ExerciseTable::set_editable(bool canEdit)
{
    if (canEdit) {
        setEditTriggers(QAbstractItemView::CurrentChanged);
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else {
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setSelectionMode(QAbstractItemView::NoSelection);
    }
}

void ExerciseTable::set_name_editable(bool can_edit) { name_editable = can_edit; }

bool ExerciseTable::set_exercise(const Exercise& e, int row)
{
    if (row >= rowCount()) return false;

    setItem(row, 1, new QTableWidgetItem(QString::number(e.duration())));

    return set_exercise_noduration(e, row);
}

bool ExerciseTable::set_exercise_noduration(const Exercise& e, int row)
{
    if (row >= rowCount()) return false;

    setItem(row, 0, new QTableWidgetItem(QString::fromStdString(e.name())));
    if (!name_editable) {
        auto name_item = item(row, 0);
        if (name_item) name_item->setFlags(name_item->flags() & ~Qt::ItemIsEditable);
    }

    setItem(row, 2, new QTableWidgetItem(QString::number(e.calories())));

    return true;
}

bool ExerciseTable::row_empty(int row)
{
    if (row >= rowCount()) return true;

    auto cur = item(row, 0);
    if (cur != nullptr && cur->text() != "") return false;

    cur = item(row, 1);
    if (cur != nullptr && cur->text() != "") return false;

    cur = item(row, 2);
    if (cur != nullptr && cur->text() != "") return false;

    return true;
}

bool ExerciseTable::has_exercise(int row)
{
    if (row_empty(row)) return false;

    auto cur = item(row, 1);
    bool ret;
    if (cur != nullptr) {
        cur->text().toDouble(&ret);
        if (!ret) return false;
    }

    cur = item(row, 2);
    if (cur != nullptr) {
        cur->text().toDouble(&ret);
        if (!ret) return false;
    }

    return true;
}

Exercise ExerciseTable::read_exercise(int row)
{
    if (row >= rowCount()) throw new std::exception("Row out of bounds");

    auto cur = item(row, 0);
    std::string name = "";
    if (cur != nullptr) name = cur->text().toStdString();

    cur = item(row, 1);
    TIME_T duration = 0;
    bool ret;
    if (cur != nullptr) duration = cur->text().toDouble(&ret);
    if(!ret) throw new std::exception("Row has no exercise");

    cur = item(row, 2);
    CAL_T cals = 0;
    if (cur != nullptr) cals = cur->text().toDouble(&ret);
    if (!ret) throw new std::exception("Row has no exercise");

    if (duration == 0) duration = 60;   //  idk
    ExerciseType et(name, cals * 60 / duration);

    return Exercise(et, duration);
}

//int ExerciseTable::findExercise(const Exercise& e);


void ExerciseTable::set_minimum_rows(int min) { min_rows = min; }
int ExerciseTable::get_minimum_rows() { return min_rows; }

void ExerciseTable::clear_row(int row)
{
    if (row >= rowCount()) return;

    setItem(row, 0, nullptr);
    setItem(row, 1, nullptr);
    setItem(row, 2, nullptr);
}

void ExerciseTable::remove_row(int row){ QTableWidget::removeRow(row); }


void ExerciseTable::clear_remove_row(int row)
{
    if (rowCount() > min_rows) {
        remove_row(row);
    }
    else {
        clear_row(row);
        move_rows_up(row);
    }
}

void ExerciseTable::clear_remove_emit_row(int row)
{
    if (has_exercise(row)) {
        Exercise e = read_exercise(row);
        emit exercise_removed(e);
    }

    clear_remove_row(row);
}

void ExerciseTable::move_rows_up(int startRow)
{
    for (int i = startRow; i < rowCount(); i++) {
        if (!row_empty(i)) continue;

        for (int j = i + 1; j < rowCount(); j++) {
            if (has_exercise(j)) {
                Exercise e = read_exercise(j);
                set_exercise(e, i);
                clear_row(j);
                break;
            }
        }
    }
}

void ExerciseTable::addExercise(const Exercise& e)
{
    for (int i = 0; i < rowCount(); i++) {
        if (row_empty(i)) {
            set_exercise(e, i);
            return;
        }
    }

    add_row();
    set_exercise(e, rowCount() - 1);
}

//void ExerciseTable::removeExercise(const Exercise& e);

void ExerciseTable::clear_table()
{
    int i = 0;
    while (i < rowCount()) {
        clear_remove_row(i);
        if (row_empty(i)) i++;
    }
}