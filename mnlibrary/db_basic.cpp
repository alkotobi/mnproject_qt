#include "db_basic.h"
#include <QDir>
#include "mnexception.h"
#include <QCoreApplication>
#include <QStandardPaths>


QString makeRelativeDbPAth(QString dbPathNoExt)
{
    QDir dir(QCoreApplication::applicationDirPath());
    QString relativeDirName = "data";
    QString absoluteDirPath = dir.absolutePath() + QDir::separator() + relativeDirName;

    if (!QDir(absoluteDirPath).exists()) {
        if (QDir().mkpath(absoluteDirPath)) {
            qDebug() << "Directory created successfully: " << absoluteDirPath;
        } else {
            qDebug() << "Failed to create directory.";
            throw MNException("Failed to create directory:"+absoluteDirPath);
        }
    } else {
        qDebug() << "Directory already exists: " << absoluteDirPath;
    }
    return absoluteDirPath + QDir::separator() + dbPathNoExt+".db";
}

QString makeDbPathInDocuments(QString dbPathNoExt) {
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir docDir(documentsPath);

    QString mnprojectPath = docDir.absoluteFilePath("mnproject");
    if (!QDir(mnprojectPath).exists()) {
        QDir().mkpath(mnprojectPath);
    }

    QString dataPath = QDir(mnprojectPath).absoluteFilePath("data");
    if (!QDir(dataPath).exists()) {
        QDir(mnprojectPath).mkdir("data");
    }

    QString fullPath = QDir(dataPath).absoluteFilePath(dbPathNoExt + ".db");
    return fullPath;
}

QString makeDbPath(QString dbPathNoExt) {
#ifdef DBPATHRELATIVE
   return makeRelativeDbPAth( dbPathNoExt) ;
#endif
#ifndef DBPATHRELATIVE
  return makeDbPathInDocuments( dbPathNoExt);
#endif
}
