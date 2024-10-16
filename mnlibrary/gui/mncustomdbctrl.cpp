#include "mncustomdbctrl.h"
#include "mnmapper.h"

MnMapper *MnCustomDbCtrl::mapper() const
{
    return _mapper;
}

void MnCustomDbCtrl::setMapper(MnMapper *newMapper)
{
    if(_mapper == newMapper){
        return;
    }
    if(_mapper){
        _mapper->removeDbCtrl(this);
    }
    _mapper = newMapper;
    _mapper->addDbCtrl(this);
}

MnCustomDbCtrl::MnCustomDbCtrl(QWidget *parent): QWidget(parent) {}



QString MnCustomDbCtrl::fieldName()
{
    return _fieldName;
}

void MnCustomDbCtrl::setFieldName(const QString &fieldName)
{
    _fieldName =fieldName;
}
