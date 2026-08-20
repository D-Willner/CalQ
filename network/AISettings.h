#pragma once
#include <QJSonObject>

class AISettings
{
private:
	std::string last_model_used;
	std::string instruction;
	std::string server_address;
	std::string port;
public:
	enum KEY{
		LAST_MODEL_USED, 
		INSTRUCTION, 
		SERVER_ADDRESS, 
		PORT
	};

	AISettings();
	AISettings(std::string lmodel, std::string inst, std::string address, std::string p);

	void set_key(KEY, std::string key_value);
	std::string get_key(KEY) const;

	static AISettings from_json(const QJsonObject&);
	QJsonObject to_json() const;
};

