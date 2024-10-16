#include "mntableview.h"
#include <QKeyEvent>


MnTableView::MnTableView( QWidget *parent) :
    QTableView(parent)
{
    tableModel = nullptr;
    previousRowIndex = QModelIndex();
}

void MnTableView::setModel(MnTableModel *model)
{
    QTableView::setModel(model);
    tableModel = model;
    previousRowIndex = model->index(0,0);
}

void MnTableView::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown ||
         (event->key() == Qt::Key_Tab && currentIndex().column() == model()->columnCount() - 1) ||
         (event->key() == Qt::Key_Backtab && currentIndex().column() == 0)))
    {
        if(tableModel->table()->state() == stEdit || tableModel->table()->state() == stInsert){
            if(!tableModel->table()->post()){
                event->ignore();
                return;
            }
        }
        previousRowIndex = currentIndex();
    }
    QTableView::keyPressEvent(event);
}

void MnTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    if(previousRowIndex != currentIndex()){
        if(tableModel->table()->state() == stEdit || tableModel->table()->state() == stInsert){
            if(!tableModel->table()->post()){
                setCurrentIndex(previousRowIndex);
                return;
            }
        }
        previousRowIndex = currentIndex();
    }
}

bool MnTableView::canMoveRow() const
{
    // Implement your condition here to determine if row movement is allowed
    // For example, you could check if the model is in a certain state or if a specific property is set.
    MnTable *mnTable = tableModel->table();
    // Add your condition logic here
    return true; // Replace this with your actual condition
}
