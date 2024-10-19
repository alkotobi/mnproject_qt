//
// Created by MERHAB NOUREDDINE on 19/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <QObject>
#include <QStyledItemDelegate>
class MnTableView;
class MnItemDelegate: public QStyledItemDelegate{
    Q_OBJECT
public:
    explicit MnItemDelegate(QObject *parent = nullptr);

    bool eventFilter(QObject *object, QEvent *event) override;
    MnTableView* tableView() const;
    virtual QWidget *createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;


};
