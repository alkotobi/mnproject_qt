#include "mndbedit.h"

MnDbEdit::MnDbEdit() {}


void MnDbEdit::setText(const QString &text)
{
    QLineEdit::setText(text);
}

QString MnDbEdit::text()
{
    return QLineEdit::text();

}
