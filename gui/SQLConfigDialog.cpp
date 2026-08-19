#include "SQLConfigDialog.h"
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>

SQLConfigDialog::SQLConfigDialog(SQLDatabase& sql_db, Settings& s, QWidget* parent)
	: sql_database(sql_db), settings(s), QDialog(parent)
{
	QPushButton* accept_btn = new QPushButton("Accept");
	QPushButton* cancel_btn = new QPushButton("Cancel");

	QLineEdit* db_name_edit = new QLineEdit(QString::fromStdString(sql_database.get_database_name()));
	QLineEdit* host_edit = new QLineEdit(QString::fromStdString(sql_database.get_server_address()));
	QLineEdit* port_edit = new QLineEdit(QString::fromStdString(sql_database.get_server_port()));
	QComboBox* db_driver_edit = new QComboBox;
	for (const std::string& driver : sql_database.driver_list()) {
		db_driver_edit->addItem(QString::fromStdString(driver));
	}
	db_driver_edit->setCurrentText(QString::fromStdString(sql_database.get_database_driver()));
	
	QLineEdit* table_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::FOODTYPE_TABLE_KEY)));
	QLineEdit* name_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::NAME_KEY)));
	QLineEdit* cal_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::CAL_KEY)));
	QLineEdit* prot_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::PROT_KEY)));
	QLineEdit* carb_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::CARB_KEY)));
	QLineEdit* fat_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::FAT_KEY)));
	QLineEdit* size_key_edit = new QLineEdit(QString::fromStdString(sql_database.get_key(SQLDatabase::SIZE_KEY)));

	QFormLayout* layout = new QFormLayout;
	layout->addRow("Database Name: ", db_name_edit);
	layout->addRow("Host: ", host_edit);
	layout->addRow("Port: ", port_edit);
	layout->addRow("Database Driver: ", db_driver_edit);

	layout->addRow("Table Key: ", table_key_edit);
	layout->addRow("Name Key: ", name_key_edit);
	layout->addRow("Calories Key: ", cal_key_edit);
	layout->addRow("Protein Key: ", prot_key_edit);
	layout->addRow("Carbohydrates Key: ", carb_key_edit);
	layout->addRow("Fat Key: ", fat_key_edit);
	layout->addRow("Size Key: ", size_key_edit);

	QHBoxLayout* button_layout = new QHBoxLayout;
	button_layout->addWidget(accept_btn,1,Qt::AlignLeft);
	button_layout->addWidget(cancel_btn,0,Qt::AlignRight);

	QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->addLayout(layout);
	main_layout->addLayout(button_layout);

	// Connect buttons
	QObject::connect(accept_btn, &QPushButton::clicked, this, [=]() {
		SQLSettings sql_settings;

		sql_settings.set_key(SQLSettings::DATABASE_NAME_KEY, db_name_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::SERVER_ADDRESS_KEY, host_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::SERVER_PORT_KEY, port_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::DATABASE_DRIVER_KEY, db_driver_edit->currentText().toStdString());
		sql_settings.set_key(SQLSettings::FOODTYPE_TABLE_KEY, table_key_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::NAME_KEY, name_key_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::CAL_KEY, cal_key_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::PROT_KEY, prot_key_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::CARB_KEY, carb_key_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::FAT_KEY, fat_key_edit->text().toStdString());
		sql_settings.set_key(SQLSettings::SIZE_KEY, size_key_edit->text().toStdString());

		sql_database.set_settings(sql_settings);
		settings.set_sql_settings(sql_settings);
		
		this->accept();
	});

	QObject::connect(cancel_btn, &QPushButton::clicked, this, [=]() {
		this->reject();
	});
}