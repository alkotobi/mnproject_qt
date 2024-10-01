//
// Created by MERHAB NOUREDDINE on 19/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mnstrings.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>


QString removeExtraSpacesForSqlText(QString& sql_text)
{
    int i = 0;
    int count = 0;
    QString result;

    while (i < sql_text.length())
    {
        if (count == 0 && sql_text[i].isSpace())
        {
            i++;
            continue;
        }
        if (sql_text[i].isSpace())
        {
            if (i == sql_text.length() - 1 || sql_text[i + 1].isSpace() || sql_text[i + 1] == ',' || sql_text[i - 1] == ',')
            {
                i++;
                continue;
            }
        }
        result += sql_text[i];
        i++;
        count++;
    }
    return result;
}




bool logQListOfStringsToFile(const QList<QStringList> &data, const QString &name) {
    QString appPath = QCoreApplication::applicationDirPath();
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("yyyyMMdd_hhmmss");

    QString fileName = appPath + QDir::separator() + name + "_" + timestamp + ".txt";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QStringList &row: data) {
            for (const QString &item: row) {
                out << item << "\t"; // Use tab as a separator between items in a row. You can change this.
            }
            out << "\n";
        }
        file.close();
        return true;
    } else {
        return false;
    }
}
