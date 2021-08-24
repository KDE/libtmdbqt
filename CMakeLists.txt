project(tmdbqt)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

set(QT_REQUIRED_VERSION 5.1.0)
find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Network)

# Used to create CMake config files
include(CMakePackageConfigHelpers)

# Used to set installation paths
include(GNUInstallDirs)

# Set the different paths
set(LIB_DESTINATION         "${CMAKE_INSTALL_FULL_LIBDIR}" CACHE PATH "Library directory name")
set(INCLUDE_DESTINATION     "${CMAKE_INSTALL_FULL_INCLUDEDIR}" CACHE PATH "The subdirectory to the header prefix")

set(CMAKE_LIBTMDBQT_VERSION_MAJOR 1)
set(CMAKE_LIBTMDBQT_VERSION_MINOR 0)
set(CMAKE_LIBTMDBQT_VERSION_PATCH 0)
set(TMDBQT_LIB_SONAME TmdbQt${QT_VERSION_MAJOR})
set(TMDBQT_LIB_SONAME_CAMEL TmdbQt${QT_VERSION_MAJOR})
set(TMDBQT_LIB_NAMESPACE TmdbQt)
set(CMAKE_LIBTMDBQT_VERSION_STRING "${CMAKE_LIBTMDBQT_VERSION_MAJOR}.${CMAKE_LIBTMDBQT_VERSION_MINOR}.${CMAKE_LIBTMDBQT_VERSION_PATCH}")

add_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

include_directories(${QT_INCLUDES})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
    ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" AND NOT WIN32))
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wcast-align -Wchar-subscripts -Wformat-security -Wno-long-long -Wpointer-arith -Wundef -Werror=return-type -Wsuggest-override")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

# Enable static build
option(TMDBQT_STATIC_BUILD "Build a static library" Off)

add_subdirectory(src)

# Enable unit testing
option(TMDBQT_ENABLE_TESTS "Enable tests" ON)
if (TMDBQT_ENABLE_TESTS)
    enable_testing()
    find_package(Qt${QT_VERSION_MAJOR} ${QT_REQUIRED_VERSION} CONFIG REQUIRED Test)
    add_subdirectory(autotests)
endif (TMDBQT_ENABLE_TESTS)

# Write LibTmdbQtConfigVersion.cmake
write_basic_package_version_file(${tmdbqt_BINARY_DIR}/${TMDBQT_LIB_SONAME_CAMEL}ConfigVersion.cmake
                                 VERSION ${CMAKE_LIBTMDBQT_VERSION_STRING}
                                 COMPATIBILITY SameMajorVersion
                                )

# Create the CMake Config files
configure_package_config_file(TmdbQtConfig.cmake.in
                              ${tmdbqt_BINARY_DIR}/${TMDBQT_LIB_SONAME_CAMEL}Config.cmake
                              INSTALL_DESTINATION ${LIB_DESTINATION}/cmake/${TMDBQT_LIB_SONAME_CAMEL}
                              PATH_VARS LIB_DESTINATION INCLUDE_DESTINATION
)

install(FILES ${tmdbqt_BINARY_DIR}/${TMDBQT_LIB_SONAME_CAMEL}Config.cmake
              ${tmdbqt_BINARY_DIR}/${TMDBQT_LIB_SONAME_CAMEL}ConfigVersion.cmake
        DESTINATION ${LIB_DESTINATION}/cmake/${TMDBQT_LIB_SONAME_CAMEL}
       )

install(EXPORT LibTmdbQtExport FILE ${TMDBQT_LIB_SONAME_CAMEL}Targets.cmake
        DESTINATION ${LIB_DESTINATION}/cmake/${TMDBQT_LIB_SONAME_CAMEL}
        NAMESPACE ${TMDBQT_LIB_NAMESPACE}::)
