#pragma once
#include <QLineEdit>
#include <QListWidget>
#include "DataBase.h"
#include "SearchInput.h"

class SearchLine : public QObject
{
	Q_OBJECT
private:
	bool results_visible;

	SearchInput* search_input;
	QListWidget* search_results;
	DataBase& database;

public:
	explicit SearchLine(DataBase& db, QObject* parent = nullptr);

	QLineEdit* get_input_widget();
	QListWidget* get_result_widget();

	bool eventFilter(QObject* object, QEvent* event) override;

public slots:
	void show_search_results(const QString& text);
	void hide_search_results();
	void update_search_results(const QString& qs);

	void clicked(QListWidgetItem* item);

	void clicked_name(std::string name);

	std::string text();
	void clear_text();

signals:
	void found(const Food& f);
	void found_meal(const Meal& m);
};

