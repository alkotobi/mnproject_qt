#ifndef FRMDBNAV_H
#define FRMDBNAV_H

#include <QWidget>
#include "mntable.h"
#include "mntableview.h"

namespace Ui {
class FrmDbNav;
}

class FrmDbNav : public QWidget
{
    Q_OBJECT

public:
    explicit FrmDbNav(QWidget *parent = nullptr);
    ~FrmDbNav();



    MnTable *table() const;
    void setTable(MnTable *newTable);

    MnTableView *tableView() const;
    void setTableView(MnTableView *newTableView);

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
    Ui::FrmDbNav *ui;
    void updateBtns();
    MnTable * _table = nullptr;
    MnTableView *_tableView = nullptr;
};

#endif // FRMDBNAV_H
