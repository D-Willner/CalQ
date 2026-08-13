#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "database/FoodType.h"

class Client : public QNetworkAccessManager
{
	Q_OBJECT
private:
	std::string model_name;
	std::string instruction;
	std::string server_address;
	std::string reasoning;

	QNetworkReply* reply;
	std::string requested_name;

public:
	explicit Client(QObject* parent = nullptr);

	const std::string& get_instruction();
	void set_instruction(const std::string& inst);

	const std::string& get_model_name();
	void set_model_name(const std::string& name);

	const std::string& get_server_address();
	void set_server_address(const std::string& address);

	//	returns false and does not request if another request is currently being processed
	bool request_models();
	bool request_food_data(const std::string& name);

signals:
	//	list of models currently registered by LMStudio
	//  first value is display name, second value is key
	void models(const std::vector<std::pair<std::string,std::string>>&);

	void food_data(const FoodType& data);

public slots:
	void reply_received();

	void models_received();

	void food_data_received();

public slots:
	void send_request(const QString& qs);
};

