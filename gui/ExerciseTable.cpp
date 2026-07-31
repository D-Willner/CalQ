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

    for (int i = 0; i < rows; i++) addRow();

    setEditable(false);
}

void ExerciseTable::insertRow(int row)
{
    QTableWidget::insertRow(row);
    QPushButton* btn = new QPushButton("-");
    setCellWidget(row, columnCount() - 1, btn);

    QObject::connect(btn, &QPushButton::clicked, this, [=]() {clearRemoveRow(currentRow());});
}

void ExerciseTable::addRow()
{
    insertRow(rowCount());
}

void ExerciseTable::setEditable(bool canEdit)
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

bool ExerciseTable::setExercise(const Exercise& e, int row)
{
    if (row >= rowCount()) return false;

    setItem(row, 1, new QTableWidgetItem(QString::number(e.duration())));

    return setExerciseNoDuration(e, row);
}

bool ExerciseTable::setExerciseNoDuration(const Exercise& e, int row)
{
    if (row >= rowCount()) return false;

    setItem(row, 0, new QTableWidgetItem(QString::fromStdString(e.name())));

    setItem(row, 2, new QTableWidgetItem(QString::number(e.calories())));

    return true;
}

bool ExerciseTable::rowIsEmpty(int row)
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

bool ExerciseTable::hasExercise(int row)
{
    if (rowIsEmpty(row)) return false;

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

Exercise ExerciseTable::readExercise(int row)
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


void ExerciseTable::setMinimumRows(int min) { minRows = min; }
int ExerciseTable::getMinimumRows() { return minRows; }

void ExerciseTable::clearRow(int row)
{
    if (row >= rowCount()) return;

    setItem(row, 0, nullptr);
    setItem(row, 1, nullptr);
    setItem(row, 2, nullptr);
}

void ExerciseTable::removeRow(int row){ QTableWidget::removeRow(row); }

void ExerciseTable::clearRemoveRow(int row)
{
    if (hasExercise(row)) {
        Exercise e = readExercise(row);
        emit exerciseRemoved(e);
    }

    if (rowCount() > minRows) {
        removeRow(row);
    }
    else {
        clearRow(row);
        moveRowsUp(row);
    }
}

void ExerciseTable::moveRowsUp(int startRow)
{
    for (int i = startRow; i < rowCount(); i++) {
        if (!rowIsEmpty(i)) continue;

        for (int j = i + 1; j < rowCount(); j++) {
            if (hasExercise(j)) {
                Exercise e = readExercise(j);
                setExercise(e, i);
                clearRow(j);
                break;
            }
        }
    }
}

void ExerciseTable::addExercise(const Exercise& e)
{
    for (int i = 0; i < rowCount(); i++) {
        if (rowIsEmpty(i)) {
            setExercise(e, i);
            return;
        }
    }

    addRow();
    setExercise(e, rowCount() - 1);
}

//void ExerciseTable::removeExercise(const Exercise& e);

void ExerciseTable::clearTable()
{
    int i = 0;
    while (i < rowCount()) {
        clearRemoveRow(i);
        if (rowIsEmpty(i)) i++;
    }
}