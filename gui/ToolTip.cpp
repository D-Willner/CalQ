#include "ToolTip.h"

ToolTip::ToolTip(const QString& text, QWidget* parent) : QLabel(parent)
{
	setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::DialogInformation).pixmap(10));
	setToolTip(text);
}