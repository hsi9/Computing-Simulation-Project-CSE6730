find_package(Git QUIET)
if (NOT GIT_FOUND)
    message( FATAL_ERROR "${BoldRed}Did not find git installed!${ColourReset}" )
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_REPO_LATEST_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${GIT_EXECUTABLE} log --pretty=format:'%H' -n 1
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE PROJECT_REPO_CURRENT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\'" "" PROJECT_REPO_CURRENT_COMMIT ${PROJECT_REPO_CURRENT_COMMIT}) # Remove quotes

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_REPO_CURRENT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --dirty --always
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE PROJECT_REPO_DIRTY_STATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (PROJECT_REPO_DIRTY_STATE MATCHES "dirty$")
    set(PROJECT_REPO_DIRTY_STATE ON)
else()
    set(PROJECT_REPO_DIRTY_STATE OFF)
endif()

string(TIMESTAMP PROJECT_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
string(TIMESTAMP PROJECT_BUILD_TIMESTAMP_ABBREVIATED "%y%m%d%H%M")


# Generate the version string based on the latest git tag and the current state of the repo (dirty/clean)
if (GIT_REPO_LATEST_TAG STREQUAL "")
    message( WARNING "No latest git tag found, defaulting to \"0.0.1\"" )
    set(VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO "0.0.1")
else()
    set(VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO "${GIT_REPO_LATEST_TAG}")
endif()

if (PROJECT_REPO_DIRTY_STATE)
    set(VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO "${VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO}.${PROJECT_BUILD_TIMESTAMP_ABBREVIATED}")
else()
    set(VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO "${VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO}.0")
endif()

message("${BoldBlue}    Current Time   : ${PROJECT_BUILD_TIMESTAMP}
    Latest Tag     : ${GIT_REPO_LATEST_TAG}
    Current Branch : ${GIT_REPO_CURRENT_BRANCH}
    Current Commit : ${PROJECT_REPO_CURRENT_COMMIT}
    Dirty State    : ${PROJECT_REPO_DIRTY_STATE}${ColourReset}
")