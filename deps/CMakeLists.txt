include(FetchContent)

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2)
FetchContent_MakeAvailable(nlohmann_json)

set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
