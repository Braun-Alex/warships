cmake_minimum_required(VERSION 3.29)
project(warships LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)

set(PROJECT_HEADERS
        entities/entities.h
        database/database.h
        entity-operations/captain/captain.h
        entity-operations/warship/warship.h
        entity-operations/warship-compartment/warship-compartment.h
)

set(PROJECT_SOURCES
        database/database.cpp
        entity-operations/captain/captain.cpp
        entity-operations/warship/warship.cpp
        entity-operations/warship-compartment/warship-compartment.cpp
)

set(UNIT_TESTS
        tests/create-read-tests/create-read-tests.cpp
        tests/update-tests/update-tests.cpp
        tests/delete-tests/delete-tests.cpp
)

include(FetchContent)

FetchContent_Declare(
        leveldb
        GIT_REPOSITORY https://github.com/google/leveldb.git
        GIT_TAG main
        FIND_PACKAGE_ARGS NAMES leveldb
)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP true
        FIND_PACKAGE_ARGS NAMES json
)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
        FIND_PACKAGE_ARGS NAMES googletest
)

FetchContent_MakeAvailable(leveldb json googletest)

enable_testing()

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${UNIT_TESTS})

target_link_libraries(${PROJECT_NAME} PRIVATE leveldb nlohmann_json::nlohmann_json GTest::gtest_main)

target_include_directories(${PROJECT_NAME} PRIVATE
        entities
        database
        entity-operations/captain
        entity-operations/warship
        entity-operations/warship-compartment
)

add_test(NAME Tests COMMAND ${PROJECT_NAME})