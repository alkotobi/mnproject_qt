include_directories(${CMAKE_SOURCE_DIR}/mnlibrary)

add_executable(mnproject_tests
        test_mnconnection.cpp
        test_mnconnection_sqlite.cpp
        test_mnconnection_postgres.cpp
)
