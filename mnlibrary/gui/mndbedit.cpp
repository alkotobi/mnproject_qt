#include "mndbedit.h"

MnDbEdit::MnDbEdit() {}

void MnDbEdit::editFinished()
{
    dataSource()->updateQry(fieldName(),dbText());
}


void MnDbEdit::setDbText(const QString &text)
{
    this->setText(text);
}

QString MnDbEdit::dbText()
{
    return this->text();

}
