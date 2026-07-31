#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qtablewidget.h"
#include "database/Exercise.h"

class ExerciseTable :
    public QTableWidget
{
    Q_OBJECT

private:

    int minRows=1;

    bool name_editable = true;

    bool row_is_free(int row);

    bool rowIsEmpty(int row);

public:
    explicit ExerciseTable(int rows, QWidget* parent = nullptr);

    void setEditable(bool canEdit);
    void set_name_editable(bool can_edit);

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
    void clear_remove_row(int row);
    void clear_remove_emit_row(int row);
    void moveRowsUp(int startRow = 0);

public slots:
    void addExercise(const Exercise& e);
    //void removeExercise(const Exercise& e); 
    void clear_table();

signals:
    void exerciseRemoved(const Exercise& e);
};

