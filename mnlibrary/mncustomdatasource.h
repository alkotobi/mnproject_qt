//
// Created by MERHAB NOUREDDINE on 23/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <QObject>

class MnCustomDbCtrl;
class MnCustomQry;
    class MnCustomDataSource:  public QObject {
        Q_OBJECT
    private:
        MnCustomQry *_qry;
    public:
        explicit MnCustomDataSource(QObject *parent= nullptr);
        virtual void setQry(MnCustomQry *qry)=0;
        virtual MnCustomQry *qry()=0;
        virtual void updateControls()=0;
        virtual void updateControls(const QString &fieldName)=0;
        virtual void updateQry(const QString &fieldName,const QString &value)=0;
        virtual void addControle(MnCustomDbCtrl *ctrl)=0;
        virtual void removeControle(MnCustomDbCtrl *ctrl)=0;


    };
