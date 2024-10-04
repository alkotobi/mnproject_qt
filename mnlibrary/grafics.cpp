//
// Created by MERHAB NOUREDDINE on 04/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "grafics.h"

QSvgWidget *createSvgWidget(const QString& svgCode,QWidget *parent) {
    QByteArray byteArray = svgCode.toUtf8();
    auto* svgWidget = new QSvgWidget(parent);
    svgWidget->load(byteArray);
    return svgWidget;
}