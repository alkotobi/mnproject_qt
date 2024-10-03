#ifndef DTM_H
#define DTM_H
#include "mnlibrary/mnconnection_sqlite.h"
#include "db_design.h"
#include "mnlibrary/mntable.h"
#include <QObject>

struct DbInfo{
    QString dbName="";
    bool isActive = false;
    bool isServer = false;
    QString server = "";
    int port =-1;
    QString userName = "";
    QString password = "";
    Provider provider;
};
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

    void creatLocalDb(const char *string);

    void regesterDb(const DbInfo &info);

    void activateDb(const QString &name);

    DbInfo activeDbName();

    DbInfo dbInfo(QString name);
};

inline Dtm *dtm;
#endif // DTM_H
