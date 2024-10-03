#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>

namespace Ui {
class FrmLogin;
}

class FrmLogin : public QDialog
{
    Q_OBJECT

public:
    explicit FrmLogin(QWidget *parent = nullptr);
    ~FrmLogin();

private slots:
    void on_btnCancel_clicked();

    void on_btnLogin_clicked();

private:
    Ui::FrmLogin *ui;
};

#endif // FRMLOGIN_H
