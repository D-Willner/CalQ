#pragma once
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include "gui/ToolTip.h"

class ToolTipLabel :
    public QFrame
{
    Q_OBJECT

private:
    ToolTip* tool_tip;
    QLabel* label;

    QHBoxLayout* layout;

public:
    explicit ToolTipLabel(const QString& tt_text, const QString& label_text, QWidget* parent = nullptr);

    void set_margin(int);
};

