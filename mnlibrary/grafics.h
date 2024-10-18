//
// Created by MERHAB NOUREDDINE on 04/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <QtSvgWidgets/QSvgWidget>

inline QString colorIcon = "blue";
QSvgWidget *createSvgWidget(const QString& svgCode, QWidget *parent= nullptr);
QByteArray modifySVGStrokeColor(const QString& svgFilePath, const QString& newColor);
QIcon iconFromSvg(const QString& svgFilePath, const QString& newColor);
QIcon iconFromSvgCode(const QString& svgCode, const QString& newColor);
