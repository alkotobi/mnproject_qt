cmake_minimum_required(VERSION 3.16)

project(mnproject VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES mnproject_ar_DZ.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mnlibrary/mnconnection.h mnlibrary/mnconnection.cpp
        mnlibrary/sqlite3.h mnlibrary/sqlite3.c
        mnlibrary/mnconnection_sqlite.h mnlibrary/mnconnection_sqlite.cpp
        mnlibrary/mnconnection_postgres.h mnlibrary/mnconnection_postgres.cpp
        mnlibrary/mnarrays.h mnlibrary/mnarrays.c
        mnlibrary/mnstrings.h mnlibrary/mnstrings.cpp
        mnlibrary/mnsql.h mnlibrary/mnsql.cpp
        mnlibrary/mnqry.h mnlibrary/mnqry.cpp
        mnlibrary/mnexception.h mnlibrary/mnexception.cpp
        mnlibrary/mndb_types.h mnlibrary/mndb_types.cpp
        mnlibrary/mndatasource.h         mnlibrary/mndatasource.cpp
        mnlibrary/mncustomdatasource.cpp mnlibrary/mncustomdatasource.h
        mnlibrary/mnqry.cpp mnlibrary/mnqry.h
        mnlibrary/mncustomquery.h mnlibrary/mncustomquery.cpp
        mnlibrary/gui/mncustomdbctrl.h mnlibrary/gui/mncustomdbctrl.cpp
        mnlibrary/gui/mndbedit.h mnlibrary/gui/mndbedit.cpp
        test.h
          ${TS_FILES}

)

    qt_add_executable(mnproject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}




    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
target_link_libraries(mnproject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(mnproject PRIVATE /Library/PostgreSQL/16/include)
target_link_libraries(mnproject PRIVATE -L/Library/PostgreSQL/16/lib)
target_link_libraries(mnproject PRIVATE libpq.a)

add_subdirectory(tests)

