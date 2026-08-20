#pragma once
#include <QDialog>
#include <QPlainTextEdit>
#include "database/Settings.h"
#include "network/Client.h"

class AIConfigDialog :
    public QDialog
{
    Q_OBJECT

private:
	Settings& settings;
    Client& client;

public:
    AIConfigDialog(Settings& settings, Client& client, QWidget* parent = nullptr);
};

