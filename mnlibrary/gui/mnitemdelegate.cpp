//
// Created by MERHAB NOUREDDINE on 19/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mnitemdelegate.h"
#include <QKeyEvent>
#include "mntableview.h"
#include "mnmapper.h"
#include "mntablemodel.h"

MnItemDelegate::MnItemDelegate(QObject *parent) : QStyledItemDelegate(parent){

}

bool MnItemDelegate::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        qDebug() <<key->key()<<" pressed";
        QModelIndex currentIndex = tableView()->currentIndex();

        // Get the column of the item
        int column = currentIndex.column();
        QWidget *editor=qobject_cast<QWidget*>(object);
        if (key->key()==Qt::Key_Tab || key->key()==Qt::Key_Enter || key->key()==Qt::Key_Return)
        {
            emit commitData(editor);
            emit closeEditor(editor, QStyledItemDelegate::NoHint);
            return true;
        }else if (key->key()==Qt::Key_Escape ){
            emit closeEditor(editor);
            return true;
        }
        if (tableView()->mapper()->model()->table()->state() == stBrowse) {
            tableView()->mapper()->edit();
            tableView()->mapper()->updateNavs();
        }
    }
    return false;
    //return QStyledItemDelegate::eventFilter(object, event);
}

MnTableView *MnItemDelegate::tableView() const {
        QWidget* widget = qobject_cast<QWidget*>(parent());
        return qobject_cast<MnTableView*>(widget);
}

QWidget *
MnItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);

        //editor->installEventFilter(filter);
        return editor;
    //return QStyledItemDelegate::createEditor(parent, option, index);
}
