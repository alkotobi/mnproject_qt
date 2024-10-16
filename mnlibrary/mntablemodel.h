#pragma once

#include <QAbstractTableModel>
#include "mntable.h"

class MnTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MnTableModel(MnTable *table, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    MnTable *table();
signals:

private:
    MnTable *mnTable;

    bool removeRows(int row, int count, const QModelIndex &parent)override;

    bool insertRows(int row, int count, const QModelIndex &parent)override;
};

