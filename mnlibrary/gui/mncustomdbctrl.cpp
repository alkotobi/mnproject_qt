#include "mncustomdbctrl.h"

MnCustomDbCtrl::MnCustomDbCtrl() {}

MnCustomDataSource *MnCustomDbCtrl::dataSource()
{
    return this->_dataSource;
}

void MnCustomDbCtrl::setDatasource(MnCustomDataSource *dataSource)
{
    if(this->_dataSource) _dataSource->removeControle(this);
    this->_dataSource = dataSource;
    _dataSource->addControle(this);
}

QString MnCustomDbCtrl::fieldName()
{
    return _fieldName;
}

void MnCustomDbCtrl::setFieldName(const QString &fieldName)
{
    _fieldName =fieldName;
}
