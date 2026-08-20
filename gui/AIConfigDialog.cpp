#include "AIConfigDialog.h"
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QFormLayout>
#include <QBoxLayout>
#include <QPushButton>
#include "ToolTipLabel.h"

AIConfigDialog::AIConfigDialog(Settings& s, Client& c, QWidget* parent)
    : QDialog(parent), settings(s), client(c) 
{
	QPushButton* accept_btn = new QPushButton("Accept");
	QPushButton* cancel_btn = new QPushButton("Cancel");

	QLineEdit* server_address_edit
		= new QLineEdit(QString::fromStdString(c.get_server_address()));
	QLineEdit* port_edit 
		= new QLineEdit(QString::fromStdString(c.get_port()));
	QPlainTextEdit* instruction_edit
		= new QPlainTextEdit(QString::fromStdString(c.get_instruction()));

	QVBoxLayout* main_layout = new QVBoxLayout(this);

	QFormLayout* text_layout = new QFormLayout;
	text_layout->addRow("Server Address: ", server_address_edit);
	text_layout->addRow("Port: ", port_edit);

	QHBoxLayout* button_layout = new QHBoxLayout;
	button_layout->addStretch();
	button_layout->addWidget(accept_btn);
	button_layout->addWidget(cancel_btn);

	main_layout->addLayout(text_layout);
	main_layout->addWidget(new ToolTipLabel("Instruction for the AI model. This will be sent to the model when making requests.", 
		"Instruction:"),0,Qt::AlignLeft);
	main_layout->addWidget(instruction_edit);
	main_layout->addLayout(button_layout);

	//	Connect buttons
	QObject::connect(accept_btn, &QPushButton::clicked, this, [=]() {
		client.set_server_address(server_address_edit->text().toStdString());
		client.set_port(port_edit->text().toStdString());
		client.set_instruction(instruction_edit->toPlainText().toStdString());
		AISettings ai_settings = settings.get_ai_settings();
		ai_settings.set_key(AISettings::SERVER_ADDRESS, server_address_edit->text().toStdString());
		ai_settings.set_key(AISettings::PORT, port_edit->text().toStdString());
		ai_settings.set_key(AISettings::INSTRUCTION, instruction_edit->toPlainText().toStdString());
		settings.set_ai_settings(ai_settings);
		this->accept();
	});

	QObject::connect(cancel_btn, &QPushButton::clicked, this, [=]() {
		this->reject();
	});
}