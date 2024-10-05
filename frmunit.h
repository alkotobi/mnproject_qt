#pragma once

#include <QDialog>

namespace Ui {
class FrmUnit;
}

class FrmUnit : public QDialog
{
    Q_OBJECT

public:
    explicit FrmUnit(QWidget *parent = nullptr);
    ~FrmUnit();

private:
    Ui::FrmUnit *ui;
};

FrmUnit *frmUnite();


inline FrmUnit *_frmUnite = nullptr;

