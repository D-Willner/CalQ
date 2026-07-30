#include "CalQ.h"

CalQ::CalQ(DataBase& db, QWidget *parent)
    : QWidget(parent), database(db)
{ 
    //setGeometry(0, 0, 800, 600);

    //setMinimumSize(800, 600);

    tabs = new QTabWidget(this);
    main_tab = new MainTab(database);
    database_tab = new DataBaseTab();
    history_tab = new HistoryTab();
    settings_tab = new SettingsTab();
    
    //tabs->setMinimumSize(600, 600);

    tabs->addTab(main_tab, "Main");
    tabs->addTab(database_tab, "Database");
    tabs->addTab(history_tab, "History");
    tabs->addTab(settings_tab, "Settings");

    layout = new QVBoxLayout();
    layout->addWidget(tabs);
    setLayout(layout);

}

CalQ::~CalQ()
{}

