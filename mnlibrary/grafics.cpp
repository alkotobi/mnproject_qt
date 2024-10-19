//
// Created by MERHAB NOUREDDINE on 04/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "grafics.h"
#include <QFile>
#include <QRegularExpressionMatchIterator>
#include <QSvgRenderer>
#include <QPainter>

QSvgWidget *createSvgWidget(const QString& svgCode,QWidget *parent) {
    QByteArray byteArray = svgCode.toUtf8();
    auto* svgWidget = new QSvgWidget(parent);
    svgWidget->load(byteArray);
    return svgWidget;
}

QByteArray modifySVGStrokeColor(const QString& svgFilePath, const QString& newColor) {
    QFile file(svgFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray svgData = file.readAll();
        file.close();

        QString s = QString::fromUtf8(svgData);
        QRegularExpression re("stroke=\"([^\"]*)\"");
        QRegularExpressionMatchIterator it = re.globalMatch(s);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                s.replace(match.captured(0), QString("stroke=\"%1\"").arg(newColor));
            }
        }

        return s.toUtf8();
    }
    return QByteArray();
}


QIcon iconFromSvg(const QString& svgFilePath, const QString& newColor) {
    QFile file(svgFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray svgData = file.readAll();
        file.close();

        QString s = QString::fromUtf8(svgData);
        QRegularExpression re("stroke=\"([^\"]*)\"");
        QRegularExpressionMatchIterator it = re.globalMatch(s);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                s.replace(match.captured(0), QString("stroke=\"%1\"").arg(newColor));
            }
        }
         re=QRegularExpression("fill=\"([^\"]*)\"");
         it = re.globalMatch(s);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                s.replace(match.captured(0), QString("fill=\"%1\"").arg(newColor));
            }
        }

        QSvgRenderer renderer(s.toUtf8());
        QPixmap pixmap(32, 32);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        renderer.render(&painter);

        return QIcon(pixmap);
    }
    return QIcon();
}

QIcon iconFromSvgCode(const QString& svgCode, const QString& newColor) {
    QString s = svgCode;
    QRegularExpression re("stroke=\"([^\"]*)\"");
    QRegularExpressionMatchIterator it = re.globalMatch(s);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        if (match.hasMatch()) {
            s.replace(match.captured(0), QString("stroke=\"%1\"").arg(newColor));
        }
    }

    QSvgRenderer renderer(s.toUtf8());
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    renderer.render(&painter);

    return QIcon(pixmap);
}
