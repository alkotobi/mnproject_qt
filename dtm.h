#ifndef DTM_H
#define DTM_H
#include "mnlibrary/mnconnection_sqlite.h"
#include "db_design.h"
#include "mnlibrary/mntable.h"
#include <QObject>
class Dtm:public QObject
{
    Q_OBJECT
private:
    mnconnection_sqlite *_connOptions= nullptr;
    MnTable *_tblDatabases = nullptr;
    mnconnection *_connMain = nullptr;
    MnTable *_tblGroup= nullptr;
    MnTable *_tblUser= nullptr;


public:
    Dtm();
    ~Dtm();
    mnconnection *connOptions();
    MnTable *tblGroup();
    MnTable *tblUser();
    MnTable *tblDatabases();

    mnconnection *connMain();
};

#endif // DTM_H
