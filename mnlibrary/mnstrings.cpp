//
// Created by MERHAB NOUREDDINE on 19/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mnstrings.h"


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