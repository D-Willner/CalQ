#include "AISettings.h"

AISettings::AISettings() : AISettings("", 
	"You will receive the name of a food or drink. \
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
The name is: ", 
"localhost", "1234") {}

AISettings::AISettings(std::string lmodel, std::string inst, std::string address, std::string p)
	: last_model_used(lmodel), instruction(inst), server_address(address), port(p)
{}

void AISettings::set_key(KEY key, std::string key_value)
{
	switch (key) {
	case LAST_MODEL_USED:
		last_model_used = key_value;
		break;
	case INSTRUCTION:
		instruction = key_value;
		break;
	case SERVER_ADDRESS:
		server_address = key_value;
		break;
	case PORT:
		port = key_value;
		break;
	}
}

std::string AISettings::get_key(KEY key) const
{
	switch (key) {
	case LAST_MODEL_USED:
		return last_model_used;	
	case INSTRUCTION:
		return instruction;
	case SERVER_ADDRESS:
		return server_address;
	case PORT:
		return port;
	default:
		throw std::invalid_argument("Invalid AISettings key");
	}
}

AISettings AISettings::from_json(const QJsonObject& jo	)
{
	AISettings settings;

	if (jo.contains("last_model_used") && jo["last_model_used"].isString())
		settings.last_model_used = jo["last_model_used"].toString().toStdString();
	if (jo.contains("instruction") && jo["instruction"].isString())
		settings.instruction = jo["instruction"].toString().toStdString();
	if (jo.contains("server_address") && jo["server_address"].isString())
		settings.server_address = jo["server_address"].toString().toStdString();
	if (jo.contains("port") && jo["port"].isString())
		settings.port = jo["port"].toString().toStdString();

	return settings;
}

QJsonObject AISettings::to_json() const
{
	QJsonObject jo;

	jo["last_model_used"] = QString::fromStdString(last_model_used);
	jo["instruction"] = QString::fromStdString(instruction);
	jo["server_address"] = QString::fromStdString(server_address);
	jo["port"] = QString::fromStdString(port);

	return jo;
}