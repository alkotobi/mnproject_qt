#pragma once

#include <QTableView>
#include "mntablemodel.h"

class MnTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MnTableView(QWidget *parent = nullptr);
    void setModel(MnTableModel *model);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool canMoveRow() const;
    MnTableModel *tableModel;
    QModelIndex previousRowIndex;
};

