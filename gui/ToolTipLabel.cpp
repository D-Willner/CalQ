#include "ToolTipLabel.h"
#include <QHBoxLayout>

ToolTipLabel::ToolTipLabel(const QString& tt_text, const QString& label_text, QWidget* parent) : QWidget(parent)
{
	tool_tip = new ToolTip(tt_text);
	label = new QLabel(label_text);

	QHBoxLayout* layout = new QHBoxLayout(this);

	layout->addWidget(tool_tip);
	layout->addWidget(label);

	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}