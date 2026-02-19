include(FetchContent)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.26.0
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(httplib)
include_directories(${httplib_SOURCE_DIR})