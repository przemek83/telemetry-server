cmake_minimum_required(VERSION 3.14)

project(telemetry-server
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Telemetry server for event tracking and analytics"
    HOMEPAGE_URL "https://github.com/przemek83/telemetry-server")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CODE_COVERAGE "Collect coverage" OFF)

if(CODE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
endif()


include(httplib.cmake)
include(json.cmake)

set(${PROJECT_NAME}_SOURCES
    src/PostHandler.h
    src/PostHandler.cpp
    src/GetHandler.h
    src/GetHandler.cpp
    src/Telemetry.h
    src/Telemetry.cpp
    src/EventHandler.h
    src/EventHandler.cpp
    src/Logger.h
    src/SimpleLogger.h
    src/SimpleLogger.cpp
)

add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}-lib nlohmann_json::nlohmann_json)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib nlohmann_json::nlohmann_json)

enable_testing()
include(Tests.cmake)
add_subdirectory(tests)
