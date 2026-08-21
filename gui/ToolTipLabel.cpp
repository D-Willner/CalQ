#include "ToolTipLabel.h"

ToolTipLabel::ToolTipLabel(const QString& tt_text, const QString& label_text, QWidget* parent) : QFrame(parent)
{
	tool_tip = new ToolTip(tt_text);
	label = new QLabel("<b>" + label_text + "</b>");

	layout = new QHBoxLayout(this);
	layout->setSpacing(5);
	layout->setContentsMargins(0, 0, 0, 0);

	layout->addWidget(tool_tip,0,Qt::AlignLeft);
	layout->addWidget(label,1,Qt::AlignLeft);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	setStyleSheet("QFrame{border-style: none; border-width: 0px; border-color: black; padding: 0px; border-radius: 0px; color: black; margin-top: 5px;}\
				   QLabel{border-style: none; border-width: 0px; margin: 0px; padding: 0px; border-radius: 0px; color: black;}");
}


void ToolTipLabel::set_margin(int i)
{
	layout->setContentsMargins(i, i, i, i);
}