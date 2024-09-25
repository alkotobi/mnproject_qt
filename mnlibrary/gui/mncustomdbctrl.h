#ifndef MNCUSTOMDBCTRL_H
#define MNCUSTOMDBCTRL_H
#include "../mncustomdatasource.h"
#include <QString>

class MnCustomDbCtrl
{
    MnCustomDataSource *_dataSource=nullptr;
    QString _fieldName="";
public slots:
    virtual void editFinished()=0;
public:
    MnCustomDbCtrl();
    MnCustomDataSource *dataSource();
    void setDatasource(MnCustomDataSource *dataSource);
    QString fieldName();
    void setFieldName(const QString& fieldName);
    virtual void setDbText(const QString& text)=0;
    virtual QString dbText()=0;
};

#endif // MNCUSTOMDBCTRL_H
