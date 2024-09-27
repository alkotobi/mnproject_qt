//
// Created by MERHAB NOUREDDINE on 27/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

//--------------------
//------mnlog---------
//--------------------

//#ifdef DEBUG_MODE
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
//#else
//#define ANSI_COLOR_RED ""
//#define ANSI_COLOR_GREEN ""
//#define ANSI_COLOR_YELLOW ""
//#define ANSI_COLOR_BLUE ""
//#define ANSI_COLOR_MAGENTA ""
//#define ANSI_COLOR_CYAN ""
//#define ANSI_COLOR_RESET ""
//#endif

typedef enum
{
    MN_INFO,
    MN_WARNING,
    MN_ERROR,
    MN_TEST,
    MN_YELLOW,
    MN_GREEN,
    MN_RED,
    MN_BLUE,
    MN_MAGENTA,
    MN_CYAN
} Mnlog_Level;

void mnlog(Mnlog_Level level, const char *fmt, ...);
void test(bool test_res, const char *msg);
