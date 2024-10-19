#include "mntableview.h"
#include "mnmapper.h"
#include <QDebug>
#include "mnitemdelegate.h"


MnTableView::MnTableView( QWidget *parent) :
    QTableView(parent)
{
    tableModel = nullptr;
    previousRowIndex = QModelIndex();
    this->setItemDelegate(new MnItemDelegate(this));

}


void MnTableView::setModel(MnTableModel *model)
{
    QTableView::setModel(model);
    tableModel = model;
    previousRowIndex = model->index(0,0);
}

void MnTableView::setMapper(MnMapper *mapper)
{
    if(mapper == this->_mapper)
        return;
    if(_mapper){
        _mapper->removeDbCtrl(this);
    }
    _mapper = mapper;
    _mapper->addDbCtrl(this);
    this->setModel(_mapper->model());

    for (int var = 0; var < _mapper->model()->table()->fields().size(); ++var) {
        if(!_mapper->model()->table()->tableDef().fields[var].is_visible)
            setColumnHidden(var,true);
    }
    setColumnHidden(0,false);
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
        this->_mapper->updateNavs();
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
        this->_mapper->updateNavs();
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

MnMapper *MnTableView::mapper() const
{
    return _mapper;
}
