//
// Created by MERHAB NOUREDDINE on 27/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#include "mnlog.h"

void mnlog(Mnlog_Level level, const char *fmt, ...)
{
    switch (level)
    {
        case MN_INFO:
            fprintf(stdout, ANSI_COLOR_GREEN "[INFO] ");
            break;
        case MN_WARNING:
            fprintf(stdout, ANSI_COLOR_YELLOW "[WARNING] ");
            break;
        case MN_ERROR:
            fprintf(stdout, ANSI_COLOR_RED "[ERROR] ");
            break;
        case MN_TEST:
            fprintf(stdout, ANSI_COLOR_CYAN "[TEST] ");
            break;
        case MN_GREEN:
            fprintf(stdout, ANSI_COLOR_CYAN "[INFO] ");
            break;
        case MN_YELLOW:
            fprintf(stdout, ANSI_COLOR_CYAN "[INFO] ");
            break;
        case MN_BLUE:
            fprintf(stdout, ANSI_COLOR_CYAN "[INFO] ");
            break;
        case MN_RED:
            fprintf(stdout, ANSI_COLOR_CYAN "[INFO] ");
            break;
        case MN_MAGENTA:
            fprintf(stdout, ANSI_COLOR_CYAN "[INFO] ");
            break;
        case MN_CYAN:
            fprintf(stdout, ANSI_COLOR_CYAN "[INFO] ");
            break;
        default:
            assert(0 && "unreachable");
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fprintf(stdout, "\n" ANSI_COLOR_RESET);
}

void test(bool test_res, const char *msg)
{
    mnlog(MN_TEST, "%s", msg);
    if (test_res)
    {
        mnlog(MN_INFO, "%s%s", msg, "  ...WORKING");
    }
    else
    {
        mnlog(MN_ERROR, "%s%s", msg, "...did not pass the test");
        abort();
    }
}
