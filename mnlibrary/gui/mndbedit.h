#ifndef MNDBEDIT_H
#define MNDBEDIT_H

#include <QLineEdit>
#include "mncustomdbctrl.h"

class MnMapper;
class MnDbEdit : public QLineEdit,MnCustomDbCtrl
{
public:
    MnDbEdit();

    // MnCustomDbCtrl interface

public:
    void setText(const QString &text) override;
    QString text() override;
};

#endif // MNDBEDIT_H
