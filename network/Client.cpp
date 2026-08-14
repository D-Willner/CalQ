#include "Client.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <memory>
#include <chrono>

using namespace std::chrono_literals;

Client::Client(QObject* parent) : QNetworkAccessManager(parent) 
{
	server_address = "http://localhost:1234";
	//instruction = "You will receive the name of a food or drink. \
Reply with an estimate for the calories, that 100g or 100ml of that food or drink contain. \
Only reply with this number and nothing else. \
The name is: ";
	//instruction = "You will receive the name of a food or drink. \
Reply with an estimate for the calories, the proteins, the carbohydrates and fats, \
that 100g or 100ml of that food or drink contain. \
Your reply must have the following format. \
In line one only write the number of calories per 100g, \
in line two only write the number of protein per 100g, \
in line three only write the number of carbohydrates per 100g \
and in line four only write the number of fats per 100g.\
Write nothing else. \
The name is: ";
	instruction = "You will receive the name of a food or drink. \
Reply with an estimate for the calories, the proteins, the carbohydrates and fats, \
that 100g of that food or drink contain. \
Your reply must have the following Json format. \
On the top level have a Json object, which contains the following fields:\
Field \"calories\" containing the number of calories per 100g, \
Field \"protein\" containing the number of protein per 100g, \
Field \"carbohydrates\" containing the number of carbohydrates per 100g, \
Field \"fats\" containing the number of fats per 100g, \
Write nothing else. The output must be in a parsable Json format. \
Do not write ```json or anything similiar! \
The name is: ";
	model_name = "google/gemma-4-e2b";
}

const std::string& Client::get_instruction() { return instruction; }
void Client::set_instruction(const std::string& inst) { instruction = inst; }

const std::string& Client::get_model_name() { return model_name; }
void Client::set_model_name(const std::string& name) { if (name != "") model_name = name; }

const std::string& Client::get_server_address() { return server_address; }
void Client::set_server_address(const std::string& address) { server_address = address; }


bool Client::request_models()
{
	if (reply) return false;

	QNetworkRequest req(QUrl(QString::fromStdString(server_address + "/api/v1/models" )));
	//req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	req.setTcpKeepAliveIdleTimeBeforeProbes(20s);
	req.setTcpKeepAliveIntervalBetweenProbes(2s);
	req.setTcpKeepAliveProbeCount(5);

	reply = get(req);

	QObject::connect(reply, &QNetworkReply::finished, this, &Client::models_received);

	return true;
}

bool Client::request_food_data(const std::string& name)
{
	if (reply) return false;
	requested_name = name;

	QNetworkRequest req(QUrl(QString::fromStdString(server_address + "/api/v1/chat")));
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	req.setTcpKeepAliveIdleTimeBeforeProbes(20s);
	req.setTcpKeepAliveIntervalBetweenProbes(2s);
	req.setTcpKeepAliveProbeCount(5);

	QJsonObject jo;
	jo["model"] = QString::fromStdString(model_name);
	jo["input"] = QString::fromStdString(instruction + name);
	jo["reasoning"] = "on";

	QJsonDocument data(jo);

	reply = post(req, data.toJson());

	QObject::connect(reply, &QNetworkReply::finished, this, &Client::food_data_received);

	return true;
}

void Client::models_received()
{
	if (!reply) return;	//	should not happen

	QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

	QObject::disconnect(reply, &QNetworkReply::finished, this, &Client::models_received);
	reply->deleteLater();
	reply = nullptr;

	if (doc.isEmpty()) return;	//	No connection possible
	if (!doc.isObject()) throw new std::exception("Wrong Json format when reading models");
	QJsonObject jo = doc.object();

	std::vector<std::pair<std::string,std::string>> model_vec;
	if(!jo.contains("models") || !jo["models"].isArray()) throw new std::exception("Wrong Json format when reading models");
	for (const QJsonValue& val : jo["models"].toArray()) {
		if (!val.isObject()) continue;
		QJsonObject model = val.toObject();

		if (!model.contains("key") || !model["key"].isString()) continue;
		QString key = model["key"].toString();

		QString display_name;
		if (!model.contains("display_name") || !model["display_name"].isString()) {
			display_name = key;
		}
		else {
			display_name = model["display_name"].toString();
		}

		model_vec.push_back({ display_name.toStdString(), key.toStdString() });
	}

	emit models(model_vec);
}

void Client::food_data_received()
{
	if (!reply) return;	//	should not happen

	QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
	
	QObject::disconnect(reply, &QNetworkReply::finished, this, &Client::food_data_received);
	reply->deleteLater();
	reply = nullptr;

	if (!doc.isObject()) throw new std::exception("Wrong Json format when reading food data");
	QJsonObject jo = doc.object();

	if(!jo.contains("output") || !jo["output"].isArray()) throw new std::exception("Wrong Json format when reading food data");

	QString response = "Error";
	for (const QJsonValue& val : jo["output"].toArray()) {
		if (!val.isObject()) continue;

		QJsonObject j = val.toObject();
		if (j.contains("type") && j["type"].isString() && j["type"].toString() == "message" 
			&& j.contains("content") && j["content"].isString()) {
			response = j["content"].toString();
		}
	}
	if (response == "Error") return;


	QFile file("food.txt");
	if (!file.open(QFile::WriteOnly)) throw new std::exception("Could not open file");
	file.write(response.toUtf8());

	QJsonDocument food_doc = QJsonDocument::fromJson(response.toUtf8());
	if (food_doc.isNull() || !food_doc.isObject()) return;
	QJsonObject food_obj = food_doc.object();

	double cals = 0;
	if (food_obj.contains("calories") && food_obj["calories"].isDouble()) 
		cals = food_obj["calories"].toDouble();

	double prot = 0;
	if (food_obj.contains("protein") && food_obj["protein"].isDouble())
		prot = food_obj["protein"].toDouble();

	double carbs = 0;
	if (food_obj.contains("carbohydrates") && food_obj["carbohydrates"].isDouble())
		carbs = food_obj["carbohydrates"].toDouble();

	double fats = 0;
	if (food_obj.contains("fats") && food_obj["fats"].isDouble())
		fats = food_obj["fats"].toDouble();

	emit food_data(FoodType(requested_name,cals,prot,carbs,fats,100));
}

// bad, only one request at a time otherwise bug
void Client::send_request(const QString& qs)
{
	QNetworkRequest req(QUrl(QString::fromStdString(server_address + "/api/v1/chat")));
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	req.setTcpKeepAliveIdleTimeBeforeProbes(20s);
	req.setTcpKeepAliveIntervalBetweenProbes(2s);
	req.setTcpKeepAliveProbeCount(5);

	QJsonObject jo;
	jo["model"] = QString::fromStdString(model_name);
	jo["input"] = QString::fromStdString(instruction) + qs;
	jo["reasoning"] = "on";

	QJsonDocument data(jo);

	if (reply) throw new std::exception("Two concurrent requests");
	reply = post(req, data.toJson());
	//reply = get(req);

	QObject::connect(reply, &QNetworkReply::finished, this, &Client::reply_received);
}

void Client::reply_received()
{
	if (!reply) return;

	QFile file("reply.txt");
	if (!file.open(QFile::WriteOnly)) throw new std::exception("Could not open file");
	auto reply_data = reply->readAll();
	file.write(reply_data);
	reply->close();
	delete reply;
	reply = nullptr;
}