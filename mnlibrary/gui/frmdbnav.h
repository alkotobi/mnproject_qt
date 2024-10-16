#ifndef FRMDBNAV_H
#define FRMDBNAV_H

#include <QWidget>

#include <QPushButton>

namespace Ui {
class FrmDbNav;
}
class MnMapper;
class FrmDbNav : public QWidget
{
    Q_OBJECT

public:
    explicit FrmDbNav(QWidget *parent = nullptr);
    ~FrmDbNav();
    void updateBtns();

    Ui::FrmDbNav *ui;
    QPushButton *btnFirst();
    QPushButton *btnNext();
    QPushButton *btnPrior();
    QPushButton *btnLast();
    QPushButton *btnRefresh();
    QPushButton *btnCancel();
    QPushButton *btnDelete();
    QPushButton *btnSave();
    MnMapper *mapper() const;
    void setMapper(MnMapper *newMapper);

private slots:
    void on_btnFirst_clicked();

    void on_btnPrior_clicked();

    void on_btnNext_clicked();

    void on_btnLast_clicked();

    void on_btnCancel_clicked();

    void on_btnDelete_clicked();

    void on_btnSave_clicked();

    void on_btnRefresh_clicked();

private:
   MnMapper *_mapper;
};

#endif // FRMDBNAV_H
