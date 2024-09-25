//
// Created by MERHAB NOUREDDINE on 23/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mndatasource.h"
#include "./gui/mncustomdbctrl.h"

void MnDataSource::setQry(MnCustomQry *qry) {
    if(_qry) _qry->removeDataSource(this);
    _qry = qry;
    qry->addDataSource(this);
}

MnCustomQry *MnDataSource::qry()
{
    return nullptr;
}

void MnDataSource::updateControls()
{
    for (int i = 0; i < controls.count(); ++i) {
        QString str = *(this->_qry->fieldByName(controls[i]->fieldName()));
        controls[i]->setDbText(str);
    }
}

void MnDataSource::updateControls(const QString &fieldName)
{
    for (int i = 0; i < controls.count(); ++i) {
        if(controls[i]->fieldName()!=fieldName) continue;
        QString str = *(this->_qry->fieldByName(controls[i]->fieldName()));
        controls[i]->setDbText(str);
    }
}

void MnDataSource::updateQry(const QString &fieldName,const QString &value)
{
    //TODO:beforeUpdateField
    *(this->_qry->fieldByName(fieldName)) = value;


}



void MnDataSource::addControle(MnCustomDbCtrl *ctrl)
{
    if(controls.contains(ctrl)) return;
    controls.append(ctrl);
}

void MnDataSource::removeControle(MnCustomDbCtrl *ctrl)
{
    controls.removeOne(ctrl);
}

MnDataSource::MnDataSource(QObject *parent) : MnCustomDataSource(parent) {

}

MnDataSource::~MnDataSource() {
  //TODO: remove connections with qry and controls
}
