//
// Created by MERHAB NOUREDDINE on 08/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mnmapper.h"
#include <QMessageBox>

MnTableModel *MnMapper::model() const
{
    return _model;
}

void MnMapper::setModel(MnTableModel *newModel)
{
    if(_model == newModel){
        return;
    }
    _model = newModel;
    QDataWidgetMapper::setModel(newModel);
}

MnMapper::MnMapper(MnTableModel *model, QObject *parent)
: QDataWidgetMapper(parent), _model(model)
{
    this->setModel(model);
    this->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void MnMapper::addDbCtrl(MnCustomDbCtrl *ctrl) {
    this->addMapping(ctrl,(int)_model->table()->fieldIndex(ctrl->fieldName()));
}

void MnMapper::removeDbCtrl(MnCustomDbCtrl *ctrl) {
    this->removeMapping(ctrl);
}

void MnMapper::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (current.row() == previous.row())
        return;
    if (!userInteract) {
        userInteract = true;
        return;
    }
    this->setCurrentModelIndex(current);
    emit rowChanged(current.row());
}

void MnMapper::removeDbCtrl(MnTableView *tableView) {
    disconnect(tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MnMapper::currentRowChanged);
    views.removeOne(tableView);
}

void MnMapper::addDbCtrl(MnTableView *tableView) {
    views.append(tableView);
    connect(tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MnMapper::currentRowChanged);
}

bool MnMapper::goTo(int row) {
    bool ret = _model->table()->goTo(row);
    if (ret) {
        this->setCurrentModelIndex(_model->index(_model->table()->rowNo(), 0));
        emit rowChanged(row);
        emit onMove();
        for (MnTableView* tableView : views)
        {
            userInteract = false;
            tableView->setCurrentIndex(_model->index(_model->table()->rowNo(), tableView->currentIndex().column()));
        }
    }
    return ret;
}

bool MnMapper::next() {
    bool ret = _model->table()->next();
    if (ret) {
        this->setCurrentModelIndex(_model->index(_model->table()->rowNo(), 0));
        emit onNext();
        emit onMove();
        emit rowChanged(_model->table()->rowNo());
        for (MnTableView* tableView : views)
        {
            userInteract = false;
            tableView->setCurrentIndex(_model->index(_model->table()->rowNo(), tableView->currentIndex().column()));
        }
    }
    return ret;
}

bool MnMapper::prior() {
    bool ret = _model->table()->prior();
    if (ret) {
        this->setCurrentModelIndex(_model->index(_model->table()->rowNo(), 0));
        emit onPrior();
        emit onMove();
        emit rowChanged(_model->table()->rowNo());
        for (MnTableView* tableView : views)
        {
            userInteract = false;
            tableView->setCurrentIndex(_model->index(_model->table()->rowNo(), tableView->currentIndex().column()));
        }
    }
    return ret; 
}

bool MnMapper::first() {
    bool ret = _model->table()->first();
    if (ret) {
        this->setCurrentModelIndex(_model->index(_model->table()->rowNo(), 0));
        emit onFirst();
        emit onMove();
        emit rowChanged(_model->table()->rowNo());
        for (MnTableView* tableView : views)
        {
            userInteract = false;
            tableView->setCurrentIndex(_model->index(_model->table()->rowNo(), tableView->currentIndex().column()));
        }
    }
    return ret;
}

bool MnMapper::last() {
    bool ret = _model->table()->last();
    if (ret) {
        this->setCurrentModelIndex(_model->index(_model->table()->rowNo(), 0));
        emit onLast();
        emit onMove();
        emit rowChanged(_model->table()->rowNo());
        for (MnTableView* tableView : views)
        {
            userInteract = false;
            tableView->setCurrentIndex(_model->index(_model->table()->rowNo(), tableView->currentIndex().column()));
        }
    }
    return ret;
}

void MnMapper::removeDbCtrl(FrmDbNav *nav) {
    navs.removeOne(nav);
    disconnect(nav->btnFirst(),&QPushButton::clicked,this,&MnMapper::first);
    disconnect(nav->btnFirst(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnLast(),&QPushButton::clicked,this,&MnMapper::last);
    disconnect(nav->btnLast(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnNext(),&QPushButton::clicked,this,&MnMapper::next);
    disconnect(nav->btnNext(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnPrior(),&QPushButton::clicked,this,&MnMapper::prior);
    disconnect(nav->btnPrior(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnRefresh(),&QPushButton::clicked,this,&MnMapper::refresh);
    disconnect(nav->btnRefresh(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnSave(),&QPushButton::clicked,this,&MnMapper::save);
    disconnect(nav->btnSave(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnDelete(),&QPushButton::clicked,this,&MnMapper::remove);
    disconnect(nav->btnDelete(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnCancel(),&QPushButton::clicked,this,&MnMapper::cancel);
    disconnect(nav->btnCancel(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    disconnect(nav->btnAdd(),&QPushButton::clicked,this,&MnMapper::add);
    disconnect(nav->btnAdd(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

}
void MnMapper::addDbCtrl(FrmDbNav *nav) {
    navs.append(nav);
    connect(nav->btnFirst(),&QPushButton::clicked,this,&MnMapper::first);
    connect(nav->btnFirst(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnLast(),&QPushButton::clicked,this,&MnMapper::last);
    connect(nav->btnLast(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnNext(),&QPushButton::clicked,this,&MnMapper::next);
    connect(nav->btnNext(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnPrior(),&QPushButton::clicked,this,&MnMapper::prior);
    connect(nav->btnPrior(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnRefresh(),&QPushButton::clicked,this,&MnMapper::refresh);
    connect(nav->btnRefresh(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnSave(),&QPushButton::clicked,this,&MnMapper::save);
    connect(nav->btnSave(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnDelete(),&QPushButton::clicked,this,&MnMapper::remove);
    connect(nav->btnDelete(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnCancel(),&QPushButton::clicked,this,&MnMapper::cancel);
    connect(nav->btnCancel(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

    connect(nav->btnAdd(),&QPushButton::clicked,this,&MnMapper::add);
    connect(nav->btnAdd(),&QPushButton::clicked,nav,&FrmDbNav::updateBtns);

}

bool MnMapper::save() {
    if (_model->table()->state() == stEdit || _model->table()->state() == stBrowse){
        emit onAfterRSave(this);
        bool ret= _model->table()->post();
        if (ret)
            emit onAfterRSave(this);
        return ret;
    }
    qDebug() << _model->table()->tableName() << ":not in edit or insert mode";
    return false;
}

bool MnMapper::edit() {
    if (_model->table()->state() == stBrowse){
        _model->table()->edit();
        emit onEdit();
        return true;
    }
    qDebug() << _model->table()->tableName() << ":not in  browse mode";
    return false;
}

bool MnMapper::cancel() {
    if (_model->table()->state() == stEdit || _model->table()->state() == stBrowse){
        emit onBeforCancel(this);
        bool ret = _model->table()->cancel();
        if (ret)
            emit onAfterCancel(this);
        return ret;
    }
    qDebug() << _model->table()->tableName() << ":not in edit or insert mode";
    return false;
}

void MnMapper::remove() {
    if(QMessageBox::question(nullptr, "CONFIRM SUPPRESSION", "EST CE QUE VOUS ETE SURE DE VOULOIRE SUPPRIMER?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return ;
    emit onBeforeRemove(this);
    _model->removeRows(_model->table()->rowNo(),1);
    emit onAfterREmove(this);
}

void MnMapper::refresh() {
    _model->table()->refresh();
    emit onRefresh();
}

void MnMapper::add()
{
    emit onBeforeAdd(this);
    _model->insertRows(_model->table()->recordCount(),1);
    emit onAfterAdd(this);
    for (MnTableView* tableView : views)
    {
        userInteract = false;
        tableView->setCurrentIndex(_model->index(_model->table()->rowNo(), tableView->currentIndex().column()));
    }
}

void MnMapper::updateNavs() {
    for (auto nav :navs) {
        nav->updateBtns();
    }
}


