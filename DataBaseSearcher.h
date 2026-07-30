#pragma once
#include <QObject>
#include <QStringList>
#include "DataBase.h"

class DataBaseSearcher : public QObject
{
	Q_OBJECT

private:
	DataBase& database;

public:
	explicit DataBaseSearcher(DataBase& db, QObject* parent = nullptr);

public slots:
	QStringList search_eatables(std::string name);
	QStringList search_exercises(std::string name);

	void emit_eatables(std::string name);
	void emit_exercises(std::string name);

signals:
	void results(const QStringList& res);
};

