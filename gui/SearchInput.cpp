#include "SearchInput.h"

SearchInput::SearchInput(const std::string& txt, QWidget* parent)
	: QLineEdit(QString::fromStdString(txt), parent), first_focus(true) {}


void SearchInput::focusInEvent(QFocusEvent* e)
{
	if (first_focus) {
		setText("");
		first_focus = false;
	}

	QLineEdit::focusInEvent(e);
}