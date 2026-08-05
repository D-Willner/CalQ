#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qlabel.h"
class ToolTip :
    public QLabel
{
    Q_OBJECT

public:
    explicit ToolTip(const QString& text, QWidget* parent = nullptr);
};

