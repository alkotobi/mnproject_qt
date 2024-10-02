cmake_minimum_required(VERSION 3.16)

project(mnproject VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(Qt6_DIR "/Users/mac/Qt/6.7.2/macos/lib/cmake/Qt6")
set(CMAKE_PREFIX_PATH "/Users/mac/Qt/6.7.2/macos/lib/cmake/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)



set(PROJECT_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ./mnlibrary/mnconnection.h mnlibrary/mnconnection.cpp
        mnlibrary/sqlite3.h mnlibrary/sqlite3.c
        mnlibrary/mnconnection_sqlite.h mnlibrary/mnconnection_sqlite.cpp
        mnlibrary/mnconnection_postgres.h mnlibrary/mnconnection_postgres.cpp
        mnlibrary/mnarrays.h mnlibrary/mnarrays.c
        mnlibrary/mnstrings.h mnlibrary/mnstrings.cpp
        mnlibrary/mnsql.h mnlibrary/mnsql.cpp
        mnlibrary/mntable.h mnlibrary/mntable.cpp
        mnlibrary/mnexception.h mnlibrary/mnexception.cpp
        mnlibrary/mndb_types.h mnlibrary/mndb_types.cpp
        mnlibrary/mndatasource.h         mnlibrary/mndatasource.cpp
        mnlibrary/mncustomdatasource.cpp mnlibrary/mncustomdatasource.h
        mnlibrary/mntable.cpp mnlibrary/mntable.h
        mnlibrary/mncustomquery.h mnlibrary/mncustomquery.cpp
        mnlibrary/gui/mncustomdbctrl.h mnlibrary/gui/mncustomdbctrl.cpp
        mnlibrary/gui/mndbedit.h mnlibrary/gui/mndbedit.cpp
        dtm.h dtm.cpp
        mnlibrary/db_basic.h mnlibrary/db_basic.cpp
        db_design.h
        mnlibrary/mnview.cpp mnlibrary/mnview.h
        test.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mnproject
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            main.cpp


    )

else()
    if(ANDROID)
        add_library(mnproject SHARED
                ${PROJECT_SOURCES}
                ${TS_FILES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(mnproject
                ${PROJECT_SOURCES}
                main.cpp
        )
    endif()
endif()


qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
target_link_libraries(mnproject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(mnproject PRIVATE /Library/PostgreSQL/16/include)
target_link_libraries(mnproject PRIVATE -L/Library/PostgreSQL/16/lib)
target_link_libraries(mnproject PRIVATE libpq.a)




if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.merhab.mnproject)
endif()
set_target_properties(mnproject PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mnproject
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mnproject)
endif()

add_subdirectory(tests/)

