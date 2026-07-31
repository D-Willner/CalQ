#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qlineedit.h"
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

