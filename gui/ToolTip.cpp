#include "ToolTip.h"

ToolTip::ToolTip(const QString& tt_text, QWidget* parent) : QLabel(parent)
{
	setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::DialogInformation).pixmap(10));
	setToolTip(tt_text);
}