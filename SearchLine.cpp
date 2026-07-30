#include "SearchLine.h"
#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>


SearchLine::SearchLine(DataBase& db, QObject* parent) : QObject(parent), database(db), results_visible(false)
{
    search_input = new SearchInput("Search");
    search_input->setMinimumWidth(200);

    search_results = new QListWidget;
    search_results->setSelectionMode(QAbstractItemView::SingleSelection);


    QObject::connect(search_input, &SearchInput::textEdited, this, &SearchLine::show_search_results);
    QObject::connect(search_input, &SearchInput::textEdited, this, &SearchLine::update_search_results);
    QObject::connect(search_input, SIGNAL(editingFinished()), this, SLOT(hide_search_results()));
    QObject::connect(search_results, &QListWidget::itemClicked, this, &SearchLine::clicked);

    search_input->installEventFilter(this);
}

QLineEdit* SearchLine::get_input_widget() { return search_input; }

QListWidget* SearchLine::get_result_widget() { return search_results; }

void SearchLine::show_search_results(const QString& text)
{
    if (results_visible == false) {
        search_results->show();
        search_results->setGeometry(search_input->geometry().x(),
            search_input->geometry().y() + search_input->height(), 100, 100);
        results_visible = true;
    }
}

void SearchLine::hide_search_results() 
{
    if (results_visible == true) {
        search_results->hide();
        results_visible = false;
    }
}

void SearchLine::update_search_results(const QString& qs) 
{
    std::string s = qs.toStdString();

    auto meals = database.meal_name_contains(s);
    auto recipes = database.recipe_name_contains(s);
    auto foodtypes = database.foodtype_name_contains(s);

    QStringList labels;

    search_results->clear();
    for (const auto& m : meals) {
        QString qs = QString::fromStdString(m.name());
        search_results->addItem(QString::fromStdString(m.name()));
        labels.append(qs);
    }

    for (const auto& r : recipes) {
        QString qs = QString::fromStdString(r.name());
        if(!labels.contains(qs)) search_results->addItem(qs);
        labels.append(qs);
    }
    for (const auto& ft : foodtypes) {
        QString qs = QString::fromStdString(ft.name());
        if (!labels.contains(qs)) search_results->addItem(qs);
        labels.append(qs);
    }
}


void SearchLine::clicked(QListWidgetItem* item)
{
    if (item == nullptr) return;


    std::string name = item->data(Qt::DisplayRole).toString().toStdString();
    clicked_name(name);
}

//  argueably should be in maintab
void SearchLine::clicked_name(std::string name)
{
    DataBase::DTYPE type = database.contains(name);
    search_input->setText("");

    if (type == DataBase::DTYPE::FOODTYPE_T) {
        FoodType ft = database.get_foodtype(name);
        emit found(Food(ft, ft.get_size()));
    }
    else if (type == DataBase::DTYPE::RECIPE_T) {
        Recipe r = database.get_recipe(name);
        emit found(r.as_food());
    }
    else if (type == DataBase::DTYPE::MEAL_T) {
        Meal m = database.get_meal(name);
        Food f(m);
        //f.set_weight(m.weight_left());
        emit found_meal(m);
    }
    else {
        //
    }
}

std::string SearchLine::text() { return search_input->text().toStdString(); }

void SearchLine::clear_text() { search_input->setText(""); }


bool SearchLine::eventFilter(QObject* object, QEvent* event)
{
    if (object == search_input && event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*> (event);
        int r = search_results->currentRow();
        switch (ke->key()) {
        case Qt::Key_Up:
            if(r>0) search_results->setCurrentRow(r - 1);
            break;
        case Qt::Key_Down:
            if (r<search_results->count()-1) search_results->setCurrentRow(r + 1);
            break;
        case Qt::Key_Return:
            clicked(search_results->item(r));
            break;

        case Qt::Key_Escape:
            hide_search_results();
            search_input->clearFocus();
            break;
        }

    }

    return QObject::eventFilter(object, event);
}