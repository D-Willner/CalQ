#include "ToolTipLabel.h"
#include <QHBoxLayout>

ToolTipLabel::ToolTipLabel(const QString& tt_text, const QString& label_text, QWidget* parent) : QFrame(parent)
{
	tool_tip = new ToolTip(tt_text);
	label = new QLabel(label_text);

	QHBoxLayout* layout = new QHBoxLayout(this);

	layout->addWidget(tool_tip,0,Qt::AlignLeft);
	layout->addWidget(label,1,Qt::AlignLeft);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}