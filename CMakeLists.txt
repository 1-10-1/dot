cmake_minimum_required(VERSION 3.26)
cmake_policy(VERSION 3.26)
cmake_policy(SET CMP0092 NEW)
project(cpplayground)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

add_subdirectory(lib)

add_library(foo)

list(APPEND SOURCE_FILES
    src/main.cpp
)

list(APPEND LIBS
    fmt
    spdlog
)

target_sources(foo
  PUBLIC
    FILE_SET CXX_MODULES FILES
      src/logger.ixx
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBS})
target_link_libraries(foo ${LIBS})

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC SPDLOG_NO_EXCEPTIONS FMT_EXCEPTIONS=0 ROOT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_include_directories(foo PRIVATE "./include")

target_compile_options(${PROJECT_NAME} PRIVATE
  # -std=c++23 -Wall -Wextra -Wpedantic -Werror -Wno-unknown-attributes -Wno-gnu-line-marker
  # $<$<CONFIG:Release>:-Ofast -march=native -mtune=native>
  # $<$<CONFIG:RelWithDebInfo>:-Ofast -march=native -mtune=native -g>
)
