#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qtablewidget.h"
#include "Exercise.h"

class ExerciseTable :
    public QTableWidget
{
    Q_OBJECT

private:
    bool row_is_free(int row);
    int minRows=1;

    bool rowIsEmpty(int row);

public:
    explicit ExerciseTable(int rows, QWidget* parent = nullptr);

    void setEditable(bool canEdit);

    bool setExercise(const Exercise& e, int row);
    bool setExerciseNoDuration(const Exercise& e, int row);
    bool hasExercise(int row);
    Exercise readExercise(int row);
    //int findExercise(const Exercise& e);

    void setMinimumRows(int min);
    int getMinimumRows();

    void insertRow(int row);
    void addRow();

    void clearRow(int row);
    void removeRow(int row);
    void clearRemoveRow(int row);
    void moveRowsUp(int startRow = 0);

public slots:
    void addExercise(const Exercise& e);
    //void removeExercise(const Exercise& e); 
    void clearTable();

signals:
    void exerciseRemoved(const Exercise& e);
};

