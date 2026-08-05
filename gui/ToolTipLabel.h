#pragma once
#include "C:\Qt\6.11.1\msvc2022_64\include\QtWidgets\qwidget.h"
#include <QLabel>
#include "gui/ToolTip.h"

class ToolTipLabel :
    public QWidget
{
    Q_OBJECT

private:
    ToolTip* tool_tip;
    QLabel* label;

public:
    explicit ToolTipLabel(const QString& tt_text, const QString& label_text, QWidget* parent = nullptr);
};

