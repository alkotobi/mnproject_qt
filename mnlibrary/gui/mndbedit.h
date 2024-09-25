#ifndef MNDBEDIT_H
#define MNDBEDIT_H

#include <QLineEdit>
#include "mncustomdbctrl.h"

class MnDbEdit : public QLineEdit,MnCustomDbCtrl
{
public:
    MnDbEdit();

    // MnCustomDbCtrl interface
public slots:
    void editFinished()override;
public:
    void setDbText(const QString &text) override;
    QString dbText() override;
};

#endif // MNDBEDIT_H
