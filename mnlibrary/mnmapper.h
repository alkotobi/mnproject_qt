//
// Created by MERHAB NOUREDDINE on 08/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <QDataWidgetMapper>
#include "mntablemodel.h"
#include "mncustomdbctrl.h"
#include "mntableview.h"
#include "frmdbnav.h"

class MnMapper:public QDataWidgetMapper  {
    Q_OBJECT
private:
    MnTableModel *_model = nullptr;
    QList<MnTableView*> views;
    QList<FrmDbNav *> navs;
    bool userInteract = false;
public:
    MnMapper(MnTableModel *model,QObject *parent = nullptr);
    void addDbCtrl(MnCustomDbCtrl *ctrl);
    void removeDbCtrl(MnCustomDbCtrl *ctrl);
    void addDbCtrl(MnTableView *tableView);
    void removeDbCtrl(MnTableView *tableView);
    void addDbCtrl(FrmDbNav *nav);
    void removeDbCtrl(FrmDbNav *nav);
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void updateNavs();
    MnTableModel *model() const;
    void setModel(MnTableModel *newModel);

public slots:
    bool goTo(int row );
    bool next();
    bool prior();
    bool first();
    bool last();
    bool save();
    bool edit();
    bool cancel();
    void remove();
    void refresh();
    void add();

signals:
    void rowChanged(int row);
    void onNext();
    void onPrior();
    void onFirst();
    void onLast();
    void onMove();
    void onRefresh();
    void onEdit();
    void onBeforeRemove(MnMapper *mapper);
    void onAfterREmove(MnMapper *mapper);
    void onBeforeSave(MnMapper *mapper);
    void onAfterRSave(MnMapper *mapper);
    void onBeforCancel(MnMapper *mapper);
    void onAfterCancel(MnMapper *mapper);
    void onBeforeAdd(MnMapper *mapper);
    void onAfterAdd(MnMapper *mapper);
};
