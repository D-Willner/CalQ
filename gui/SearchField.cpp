#include "SearchField.h"
#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>


SearchField::SearchField(QObject* parent) : QObject(parent), results_visible(false)
{
    search_input = new SearchInput("Search");
    search_input->setMinimumWidth(200);

    search_results = new QListWidget;
    search_results->setSelectionMode(QAbstractItemView::SingleSelection);
    search_results->setFixedSize(200, 200);
    search_results->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    search_results->hide();


    QObject::connect(search_input, &SearchInput::textEdited, this, &SearchField::show_search_results);
    QObject::connect(search_input, &SearchInput::textEdited, this, &SearchField::text_changed);
    QObject::connect(search_input, SIGNAL(editingFinished()), this, SLOT(hide_search_results()));
    QObject::connect(search_results, &QListWidget::itemClicked, this, &SearchField::clicked);

    search_input->installEventFilter(this);
}

QLineEdit* SearchField::get_input_widget() { return search_input; }

QListWidget* SearchField::get_result_widget() { return search_results; }

void SearchField::text_changed(const QString& qs)
{
    emit request(qs.toStdString());
}

void SearchField::show_search_results(const QString& text)
{
    if (results_visible == false) {
        search_results->show();
        search_results->setGeometry(search_input->geometry().x(),
            search_input->geometry().y() + search_input->height(), 200, 200);
        results_visible = true;
    }
}

void SearchField::hide_search_results()
{
    if (results_visible == true) {
        search_results->hide();
        results_visible = false;
    }
}

void SearchField::update_search_results(const QStringList& results)
{
    search_results->clear();
    for (const QString& qs : results) {
        search_results->addItem(qs);
    }
}

void SearchField::clicked(QListWidgetItem* item)
{
    if (item == nullptr) return;


    std::string name = item->data(Qt::DisplayRole).toString().toStdString();
    clicked_name(name);
}

void SearchField::clicked_name(std::string name)
{
    search_input->setText("");

    emit found(name);

    search_input->clear();
}

std::string SearchField::text() { return search_input->text().toStdString(); }

void SearchField::clear_text() { search_input->setText(""); }


bool SearchField::eventFilter(QObject* object, QEvent* event)
{
    if (object == search_input && event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*> (event);
        int r = search_results->currentRow();
        switch (ke->key()) {
        case Qt::Key_Up:
            if (r > 0) search_results->setCurrentRow(r - 1);
            break;
        case Qt::Key_Down:
            if (r < search_results->count() - 1) search_results->setCurrentRow(r + 1);
            break;
        case Qt::Key_Return:
            if(results_visible) clicked(search_results->item(r));
            break;

        case Qt::Key_Escape:
            hide_search_results();
            search_input->clearFocus();
            break;
        }

    }

    return QObject::eventFilter(object, event);
}