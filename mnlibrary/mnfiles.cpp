//
// Created by MERHAB NOUREDDINE on 29/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mnfiles.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

void stringToFile(const QString& content, const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
        qDebug() << "File saved successfully.";
    } else {
        qDebug() << "Error opening file for writing.";
    }
}

QString pathFromFilePath(QString filePath) {
    QFileInfo fileInfo(filePath);
    QString pathOnly = fileInfo.path() +QDir::separator();
    return pathOnly;
}



QString stringFromFile(const QString &filePath) {
    QFile file(filePath);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        return in.readAll();
    } else {
        return QString();
    }
}
