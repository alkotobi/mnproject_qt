//
// Created by MERHAB NOUREDDINE on 23/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include "QObject"

class MnCustomDataSource;
class MnCustomQry : public QObject {
    Q_OBJECT
public:
    MnCustomQry(QObject *parent= nullptr);
    virtual void addDataSource(MnCustomDataSource *dts)=0;
    virtual void removeDataSource(MnCustomDataSource *dts)=0;
    virtual QString* fieldByName(const QString& name)=0;
    virtual QString* fieldByInd(const int index)=0;
};
