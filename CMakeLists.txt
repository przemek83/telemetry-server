cmake_minimum_required(VERSION 3.14)

project(callstack
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "TODO: description"
    HOMEPAGE_URL "https://github.com/przemek83/callstack")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# option(CODE_COVERAGE "Collect coverage" OFF)

# if(CODE_COVERAGE)
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 --coverage")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
# endif()

include(FetchContent)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.20.0
)

include_directories(${httplib_SOURCE_DIR})

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2 # Use the latest stable version
)

FetchContent_MakeAvailable(json)

# FetchContent_GetProperties(httplib)
# if(NOT httplib_POPULATED)
#     FetchContent_Populate(httplib)
# endif()


# FetchContent_MakeAvailable(httplib)

# include_directories(${FETCHCONTENT_BASE_DIR}/httplib-src/)

# FetchContent_MakeAvailable(httplib)


# find_package(httplib)
# target_link_libraries(your_project PUBLIC Crow::Crow)

# include(CMakeLists.txt.callstack)

# file(COPY <TODO: file> DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY <TODO: dir> DESTINATION ${CMAKE_BINARY_DIR})

set(${PROJECT_NAME}_SOURCES
    src/PostHandler.h
    src/PostHandler.cpp
    src/GetHandler.h
    src/GetHandler.cpp
    src/Telemetry.h
    src/Telemetry.cpp
    src/EventHandler.h
    src/EventHandler.cpp
)

# Create default binaries.
add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES})
# target_include_directories(${PROJECT_NAME}-lib PRIVATE ${httplib_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}-lib nlohmann_json::nlohmann_json)

add_executable(${PROJECT_NAME} src/main.cpp)
# target_include_directories(${PROJECT_NAME} PRIVATE ${httplib_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib nlohmann_json::nlohmann_json)

enable_testing()
include(Tests.cmake)
add_subdirectory(tests)
