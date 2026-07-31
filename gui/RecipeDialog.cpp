#include "RecipeDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

RecipeDialog::RecipeDialog(const Recipe& r, DataBase& db, QWidget* parent, Qt::WindowFlags f)
	: QDialog(parent, f), database(db), rec(r) 
{
	name_line = new QLineEdit;
	name_line->setMinimumWidth(200);
	name_line->setText(QString::fromStdString(rec.name()));

	food_table = new FoodTable(5);
	food_table->add_recipe(r);

	accept_btn = new QPushButton("Accept");
	toss_btn = new QPushButton("Keep previous");

	QHBoxLayout* layout_top = new QHBoxLayout;
	layout_top->addWidget(new QLabel("Name:"), 0, Qt::AlignLeft);
	layout_top->addWidget(name_line, 1, Qt::AlignLeft);

	QHBoxLayout* layout_bot = new QHBoxLayout;
	layout_bot->addWidget(accept_btn, 0, Qt::AlignLeft);
	layout_bot->addWidget(toss_btn, 1, Qt::AlignLeft);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(layout_top);
	layout->addWidget(food_table);
	layout->addLayout(layout_bot);

	setLayout(layout);

	setWindowModality(Qt::ApplicationModal);

	QObject::connect(toss_btn, &QPushButton::clicked, this, &QDialog::close);
	QObject::connect(accept_btn, &QPushButton::clicked, this, &RecipeDialog::handle_accept);
}


void RecipeDialog::handle_accept()
{
	std::string name = name_line->text().toStdString();
	auto ing = food_table->read_ingredients();
	if (name == "" || ing.size() == 0) close();

	Recipe r(name, ing);
	database.overwrite(r);
	for (const Food& f : ing) {
		database.add(f.food_type());
	}

	close();
}