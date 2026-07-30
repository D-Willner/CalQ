#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include "SearchInput.h"
#include <qlistwidget.h>

class SearchField :
    public QObject
{
	Q_OBJECT
private:
	bool results_visible;

	SearchInput* search_input;
	QListWidget* search_results;

public:
	explicit SearchField(QObject* parent = nullptr);

	QLineEdit* get_input_widget();
	QListWidget* get_result_widget();

	bool eventFilter(QObject* object, QEvent* event) override;

protected slots:
	void text_changed(const QString& qs);

public slots:
	void show_search_results(const QString& text);
	void hide_search_results();
	void update_search_results(const QStringList& results);

	void clicked(QListWidgetItem* item);

	void clicked_name(std::string name);

	std::string text();
	void clear_text();

signals:
	void found(std::string name);
	void request(std::string name);
};

