include_directories(${tmdbqt_SOURCE_DIR}/src ${tmdbqt_BINARY_DIR}/src)

macro(TMDBQT_TESTS)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test ${TMDBQT_LIB_SONAME})
        add_test(tmdb-${_testname} ${_testname})
    endforeach()
endmacro()

TMDBQT_TESTS(
   searchtest
)
