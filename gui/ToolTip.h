#pragma once
#include "qlabel.h"
class ToolTip :
    public QLabel
{
    Q_OBJECT

public:
    explicit ToolTip(const QString& tt_text, QWidget * parent = nullptr);
};

