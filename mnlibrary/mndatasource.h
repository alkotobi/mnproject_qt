//
// Created by MERHAB NOUREDDINE on 23/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <QObject>
#include "mncustomdatasource.h"
#include "mncustomquery.h"
#include <QList>

class MnDataSource:  public MnCustomDataSource {
Q_OBJECT
private:
    MnCustomQry *_qry;
    QList<MnCustomDbCtrl*> controls;
public:
    explicit MnDataSource(QObject *parent);
    ~MnDataSource() override;
    void setQry(MnCustomQry *qry) override;
    MnCustomQry *qry() override;



    // MnCustomDataSource interface
public:
    void updateControls()override;
    void updateControls(const QString &fieldName)override;
    virtual void updateQry(const QString &fieldName,const QString &value)override;
    void addControle(MnCustomDbCtrl *ctrl)override;
    void removeControle(MnCustomDbCtrl *ctrl)override;
};
