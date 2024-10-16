#pragma  once
#include "../mncustomdatasource.h"
#include <QString>
#include <QWidget>

class MnMapper;
class MnCustomDbCtrl:public QWidget
{
private:
    MnMapper *_mapper = nullptr;
    QString _fieldName="";

public:
    MnCustomDbCtrl(QWidget *parent= nullptr);
    QString fieldName();
    void setFieldName(const QString& fieldName);
    virtual void setText(const QString& text)=0;
    virtual QString text()=0;
    MnMapper *mapper() const;
    void setMapper(MnMapper *newMapper);
};


