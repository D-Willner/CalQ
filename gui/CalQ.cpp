#include "CalQ.h"
#include <QDir>

CalQ::CalQ(DataBase& db, Settings& s, QWidget* parent)
    : QWidget(parent), database(db), settings(s)
{ 
    //setGeometry(0, 0, 800, 600);

    //setMinimumSize(800, 600);

    tabs = new QTabWidget(this);
    main_tab = new MainTab(database, settings);
    database_tab = new DataBaseTab(database);
    history_tab = new HistoryTab(database, settings);
    settings_tab = new SettingsTab(settings);
    
    //tabs->setMinimumSize(600, 600);

    tabs->addTab(main_tab, "Main");
    tabs->addTab(database_tab, "Database");
    tabs->addTab(history_tab, "History");
    tabs->addTab(settings_tab, "Settings");

    layout = new QVBoxLayout();
    layout->addWidget(tabs);
    setLayout(layout);

    QObject::connect(tabs, &QTabWidget::currentChanged,
        this, [=](int index) { if (index == tabs->indexOf(database_tab)) database_tab->update_tables(); });

    QObject::connect(tabs, &QTabWidget::currentChanged,
        this, [=](int index) {
            if (index == tabs->indexOf(database_tab) && settings.is_enabled_AI()) {
                database_tab->LMS_server_running_dispatch();
                database_tab->fetch_models();
            }
    });
}

CalQ::~CalQ()
{}

