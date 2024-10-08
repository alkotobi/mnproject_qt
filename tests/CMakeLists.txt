cmake_minimum_required(VERSION 3.16)

project(test_sqlite VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/mnlibrary)


add_executable(test_sqlite
        test_mnconnection_sqlite.cpp
        ../mnlibrary/mnconnection_sqlite.cpp
        ../mnlibrary/mnconnection.cpp
        ../mnlibrary/sqlite3.c
        ../mnlibrary/mnlog.cpp

)
target_link_libraries(test_sqlite PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(test_sqlite PRIVATE /Library/PostgreSQL/16/include)
target_link_libraries(test_sqlite PRIVATE -L/Library/PostgreSQL/16/lib)
target_link_libraries(test_sqlite PRIVATE libpq.a)
add_executable(test_postgres
        test_mnconnection_postgres.cpp
        ../mnlibrary/mnconnection_postgres.cpp
        ../mnlibrary/mnconnection.cpp
        ../mnlibrary/mnarrays.c
        ../mnlibrary/mnlog.cpp
)
target_link_libraries(test_postgres PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(test_postgres PRIVATE /Library/PostgreSQL/16/include)
target_link_libraries(test_postgres PRIVATE -L/Library/PostgreSQL/16/lib)
target_link_libraries(test_postgres PRIVATE libpq.a)

add_executable(test_mntable
        test_mntable.cpp
        ../mnlibrary/mnconnection_postgres.cpp
        ../mnlibrary/mnconnection.cpp
        ../mnlibrary/mnarrays.c
        ../mnlibrary/sqlite3.c
        ../mnlibrary/mntable.cpp
        ../mnlibrary/mndb_types.cpp
        ../mnlibrary/mncustomquery.cpp
        ../mnlibrary/mnsql.cpp
        ../mnlibrary/mnstrings.cpp
        ../mnlibrary/mnlog.cpp
        ../mnlibrary/mnconnection_sqlite.cpp
        ../mnlibrary/mnview.cpp

)
target_link_libraries(test_mntable PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(test_mntable PRIVATE /Library/PostgreSQL/16/include)
target_link_libraries(test_mntable PRIVATE -L/Library/PostgreSQL/16/lib)
target_link_libraries(test_mntable PRIVATE libpq.a)
