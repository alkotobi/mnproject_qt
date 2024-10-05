#include "mntablemodel.h"

MnTableModel::MnTableModel(MnTable *table, QObject *parent)
    : QAbstractTableModel(parent), mnTable(table)
{
}

int MnTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    int c = mnTable->recordCount();
    return mnTable->recordCount();
}

int MnTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    int c = mnTable->fields().count();
    return mnTable->fields().count();
}

QVariant MnTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString d = mnTable->fieldByInd(index.column());
        mnTable->goTo(index.row());
        return mnTable->fieldByInd(index.column());
    }
    return QVariant();
}

QVariant MnTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role!= Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        return mnTable->fields()[section];
    }
    return QVariant();
}

bool MnTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row();
        int col = index.column();
        mnTable->goTo(row);
        mnTable->edit();
        mnTable->setFieldValue(col, value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags MnTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        return defaultFlags | Qt::ItemIsEditable;
    else
        return defaultFlags;
}

MnTable *MnTableModel::table()
{
    return mnTable;
}
