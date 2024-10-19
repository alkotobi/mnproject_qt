#pragma once

#include <QTableView>
#include "mntablemodel.h"
#include <QEvent>
#include <QObject>
#include <QStyledItemDelegate>
#include <QKeyEvent>
#include <QLineEdit>

class MnMapper;
//class MnTableView ;
//class ItemDelegate : public QStyledItemDelegate
//{
//    Q_OBJECT
//public:
//    explicit ItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent)
//    {
//
//    }
//
//    bool eventFilter(QObject *object, QEvent *event) override
//    {
//        if (event->type() == QEvent::KeyPress) {
//            QKeyEvent* key = static_cast<QKeyEvent*>(event);
//
//            qDebug() <<key->key()<<" pressed";
//            QModelIndex currentIndex = tableView()->currentIndex();
//
//            // Get the column of the item
//            int column = currentIndex.column();
//            QWidget *editor=qobject_cast<QWidget*>(object);
//            if (key->key()==Qt::Key_Tab || key->key()==Qt::Key_Enter || key->key()==Qt::Key_Return)
//            {
//                emit commitData(editor);
//                emit closeEditor(editor, QStyledItemDelegate::NoHint);
//                return true;
//            }else if (key->key()==Qt::Key_Escape ){
//                emit closeEditor(editor);
//                return true;
//            }
//
//        }
//        return false;
//    }
//
//    virtual QWidget *createEditor(QWidget *parent,
//                                  const QStyleOptionViewItem &option,
//                                  const QModelIndex &index) const override
//    {
//        QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
//
//        //editor->installEventFilter(filter);
//        return editor;
//    }
//
//private:
//    // Helper function to get the QTableView pointer
//    MnTableView* tableView() const
//    {
//        QWidget* widget = qobject_cast<QWidget*>(parent());
//        return qobject_cast<MnTableView*>(widget);
//    }
//};

class MnTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MnTableView(QWidget *parent = nullptr);
    void setModel(MnTableModel *model);
    void setMapper(MnMapper *mapper);

    MnMapper *mapper() const;

   // bool eventFilter(QObject *object, QEvent *event)override;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool canMoveRow() const;
    MnTableModel *tableModel;
    QModelIndex previousRowIndex;
    MnMapper *_mapper;
};

