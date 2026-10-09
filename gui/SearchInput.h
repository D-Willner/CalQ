#pragma once
#include "qlineedit.h"
class SearchInput :
    public QLineEdit
{
    Q_OBJECT

private:
    bool first_focus;

public:
    explicit SearchInput(const std::string& txt = "Search", QWidget* parent = nullptr);

    void focusInEvent(QFocusEvent* e) override;
};

