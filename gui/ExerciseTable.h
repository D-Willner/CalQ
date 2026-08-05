#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qtablewidget.h"
#include "database/Exercise.h"

class ExerciseTable :
    public QTableWidget
{
    Q_OBJECT

private:

    int min_rows=1;

    bool name_editable = true;

    bool row_is_free(int row);

    bool row_empty(int row);

public:
    explicit ExerciseTable(int rows, QWidget* parent = nullptr);

    void set_editable(bool canEdit);
    void set_name_editable(bool can_edit);

    bool set_exercise(const Exercise& e, int row);
    bool set_exercise_noduration(const Exercise& e, int row);
    bool has_exercise(int row);
    Exercise read_exercise(int row);
    //int findExercise(const Exercise& e);

    void set_minimum_rows(int min);
    int get_minimum_rows();

    void insert_row(int row);
    void add_row();

    void clear_row(int row);
    void remove_row(int row);
    void clear_remove_row(int row);
    void clear_remove_emit_row(int row);
    void move_rows_up(int startRow = 0);

public slots:
    void addExercise(const Exercise& e);
    //void removeExercise(const Exercise& e); 
    void clear_table();

signals:
    void exercise_removed(const Exercise& e);
};

