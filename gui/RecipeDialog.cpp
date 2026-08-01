#include "RecipeDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

RecipeDialog::RecipeDialog(const Recipe& r, DataBase& db, QWidget* parent, Qt::WindowFlags f)
	: QDialog(parent, f), database(db), db_searcher(db,this), rec(r) 
{
	name_line = new QLineEdit;
	name_line->setMinimumWidth(200);
	name_line->setText(QString::fromStdString(rec.name()));

	search_field = new SearchField;

	food_table = new FoodTable(5,FoodTable::FACTOR);
	food_table->add_recipe(r);
	food_table->set_amount_adjust(true);

	accept_btn = new QPushButton("Accept");
	accept_btn->setAutoDefault(false);
	toss_btn = new QPushButton("Keep previous");
	toss_btn->setAutoDefault(false);

	QHBoxLayout* layout_top = new QHBoxLayout;
	layout_top->addWidget(new QLabel("Name:"), 0, Qt::AlignLeft);
	layout_top->addWidget(name_line, 1, Qt::AlignLeft);
	layout_top->addWidget(search_field->get_input_widget(), 0, Qt::AlignRight);

	QHBoxLayout* layout_bot = new QHBoxLayout;
	layout_bot->addWidget(accept_btn, 0, Qt::AlignLeft);
	layout_bot->addWidget(toss_btn, 1, Qt::AlignLeft);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(layout_top);
	layout->addWidget(food_table);
	layout->addLayout(layout_bot);

	setLayout(layout);
	search_field->get_result_widget()->setParent(this);

	setWindowModality(Qt::ApplicationModal);

	QObject::connect(toss_btn, &QPushButton::clicked, this, &QDialog::close);
	QObject::connect(accept_btn, &QPushButton::clicked, this, &RecipeDialog::handle_accept);
	QObject::connect(search_field, &SearchField::request, &db_searcher, &DataBaseSearcher::emit_foodtypes);
	QObject::connect(&db_searcher, &DataBaseSearcher::results, search_field, &SearchField::update_search_results);
	///*
	QObject::connect(search_field, &SearchField::found, 
		this, [this](std::string name) {
			if (database.contains(name) == DataBase::FOODTYPE_T) {
				FoodType ft = database.get_foodtype(name);
				food_table->add_food(Food(ft, ft.get_size()));
			}
		}
	);
	//*/
	setFocusPolicy(Qt::ClickFocus);
}


void RecipeDialog::handle_accept()
{
	std::string name = name_line->text().toStdString();
	auto ing = food_table->read_ingredients();
	if (name == "" || ing.size() == 0) close();

	Recipe r(name, ing);
	emit found(r);

	close();
}