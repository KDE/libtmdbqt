include_directories(${tmdbqt_SOURCE_DIR}/src ${tmdbqt_BINARY_DIR}/src)

macro(TMDBQT_TESTS)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt5::Core Qt5::Test ${TMDBQT_LIB_SONAME})
        add_test(tmdb-${_testname} ${_testname})
    endforeach()
endmacro()

TMDBQT_TESTS(
   searchtest
)
