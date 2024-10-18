cmake_minimum_required(VERSION 3.28)
project(QtRpt)

cmake_minimum_required(VERSION 3.10)


project(QtRpt VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/Users/mac/Qt/6.7.2/macos/lib/cmake/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Xml Qml Sql Charts Widgets PrintSupport Core5Compat  )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Xml Qml Sql Charts Widgets PrintSupport Core5Compat )



# Set include path
set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INCLUDE_PATH} "./zint")

# Set debug and release destination directories
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    set(DEST_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/debug")
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#    set(DEST_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/release")
#endif()

# Set defines
option(QTRPT_LIBRARY "Build QtRPT as a library" OFF)
if(QTRPT_LIBRARY)
    add_definitions(-DQTRPT_LIBRARY)
endif()

option(NO_BARCODE "Build QtRPT without BARCODE support" OFF)
if(NO_BARCODE)
    add_definitions(-DNO_BARCODE)
endif()

option(QXLSX_LIBRARY "Build QtRPT with XLSX support" ON)
if(QXLSX_LIBRARY)
    add_definitions(-DQXLSX_LIBRARY)
endif()

add_definitions(-DNO_PNG -DZINT_VERSION=\"2.4.4\" )

file(GLOB_RECURSE ZINT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zint/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/zint/*.cpp")
file(GLOB_RECURSE ZINT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/zint/*.h")

# Set source files
set(SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/qtrpt.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptDsPlugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptSql.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptFieldObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptBandObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptPageObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptCrossTabObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptScriptEngine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptDsInline.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RptDsAbstract.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CommonClasses.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/numbertoarabicword.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Barcode.cpp
        #xls
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocpropscore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocpropsapp.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxrelationships.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxutility.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxsharedstrings.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcontenttypes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxtheme.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxformat.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxstyles.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxworkbook.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxabstractsheet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxworksheet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxchartsheet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxzipwriter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdrawing.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxzipreader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocument.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcell.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdatavalidation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellreference.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellrange.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxrichstring.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxconditionalformatting.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcolor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxnumformatparser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdrawinganchor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxmediafile.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxabstractooxmlfile.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxchart.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxsimpleooxmlfile.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellformula.cpp
        #zint
        ${ZINT_SOURCES}
)

# Set header files
set(HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/qtrpt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptDsPlugin.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qtrptnamespace.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptSql.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptFieldObject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptBandObject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptPageObject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptCrossTabObject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptScriptEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptDsInline.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RptDsAbstract.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CommonClasses.h
        ${CMAKE_CURRENT_SOURCE_DIR}/numbertoarabicword.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Barcode.h
        #xls
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocpropscore_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocpropsapp_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxrelationships_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxutility_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxsharedstrings_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcontenttypes_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxtheme_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxformat.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxworkbook.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxstyles_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxabstractsheet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxabstractsheet_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxworksheet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxworksheet_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxchartsheet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxchartsheet_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxzipwriter_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxworkbook_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxformat_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxglobal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdrawing_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxzipreader_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocument.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdocument_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcell.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcell_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdatavalidation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdatavalidation_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellreference.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellrange.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxrichstring_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxrichstring.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxconditionalformatting.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxconditionalformatting_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcolor_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxnumformatparser_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxdrawinganchor_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxmediafile_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxabstractooxmlfile.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxabstractooxmlfile_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxchart.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxchart_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxsimpleooxmlfile_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellformula.h
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsxcellformula_p.h
        #zint
        ${ZINT_HEADERS}
)

# Process resource file
qt6_add_resources(RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../QtRPT/imagesRpt.qrc)

# Create library or executable target
add_library(QtRpt ${SOURCE_FILES} ${HEADER_FILES})
# Or if it's an executable:
# add_executable(YourExecutableName ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})

# Link with Qt libraries
target_link_libraries(QtRpt Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Core5Compat
        Qt${QT_VERSION_MAJOR}::GuiPrivate
)
# if(NOT NO_BARCODE)
#     include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/zint-2.4.4/backend_qt4"
#             "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/zint-2.4.4/backend")
#     link_directories("${DEST_DIRECTORY}/lib")
#     target_link_libraries(QtRpt QtZint)
# endif()

# if(QXLSX_LIBRARY)
#     include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/QtXlsx")
#     link_directories("${DEST_DIRECTORY}/lib")
#     target_link_libraries(QtRpt QtXlsx)
# endif()
