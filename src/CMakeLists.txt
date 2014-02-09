set(CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
		"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
		("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" AND NOT WIN32))
	# Linker warnings should be treated as errors
	set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--fatal-warnings ${CMAKE_SHARED_LINKER_FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings ${CMAKE_MODULE_LINKER_FLAGS}")

	# Do not allow undefined symbols, even in non-symbolic shared libraries
	set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_MODULE_LINKER_FLAGS}")
endif()

configure_file(version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)
configure_file(tmdbqt_export.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/tmdbqt_export.h)

set(tmdbqt_SRCS
   themoviedbapi.cpp
   searchjob.cpp
   creditsjob.cpp
   configuration.cpp
   moviedblist.cpp
   moviedb.cpp
   personlist.cpp
   person.cpp
)


if (TMDBQT_STATIC_BUILD)
    add_library(${TMDBQT_LIB_SONAME} STATIC ${tmdbqt_SRCS})
    set_target_properties(${TMDBQT_LIB_SONAME} PROPERTIES
        VERSION ${CMAKE_LIBTMDBQT_VERSION_STRING}
        SOVERSION ${CMAKE_LIBTMDBQT_VERSION_MAJOR}
    )
else (TMDBQT_STATIC_BUILD)
    add_library(${TMDBQT_LIB_SONAME} SHARED ${tmdbqt_SRCS})
    set_target_properties(${TMDBQT_LIB_SONAME} PROPERTIES
        VERSION ${CMAKE_LIBTMDBQT_VERSION_STRING}
        SOVERSION ${CMAKE_LIBTMDBQT_VERSION_MAJOR}.${CMAKE_LIBTMDBQT_VERSION_MINOR}
        LINK_INTERFACE_LIBRARIES ""
        DEFINE_SYMBOL TMDBQT_LIB_MAKEDLL
    )
endif(TMDBQT_STATIC_BUILD)

if(Qt5Core_FOUND)
  set_property(TARGET ${TMDBQT_LIB_SONAME} APPEND PROPERTY COMPILE_DEFINITIONS QT_DISABLE_DEPRECATED_BEFORE=0)
endif()

target_link_libraries(${TMDBQT_LIB_SONAME}
PUBLIC
  Qt5::Core
PRIVATE
  Qt5::Network
)
if (Qt5Core_FOUND)
target_include_directories(${TMDBQT_LIB_SONAME} INTERFACE $<INSTALL_INTERFACE:${INCLUDE_DESTINATION}/${TMDBQT_LIB_SONAME}>)
else ()
target_include_directories(${TMDBQT_LIB_SONAME} INTERFACE $<INSTALL_INTERFACE:${INCLUDE_DESTINATION}>)
endif ()
install(TARGETS ${TMDBQT_LIB_SONAME} EXPORT LibTmdbQtExport ARCHIVE DESTINATION ${LIB_DESTINATION}
                               LIBRARY DESTINATION ${LIB_DESTINATION}
                               RUNTIME DESTINATION bin)
set(TMDBQT_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/tmdbqt_export.h
   themoviedbapi.h
   searchjob.h
   creditsjob.h
   configuration.h
   moviedblist.h
   moviedb.h
   personlist.h
   person.h
)

if (Qt5Core_FOUND)
  install(FILES ${TMDBQT_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    DESTINATION ${INCLUDE_DESTINATION}/${TMDBQT_LIB_SONAME}/tmdbqt/ COMPONENT Devel
  )
else()
  install(FILES ${TMDBQT_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    DESTINATION ${INCLUDE_DESTINATION}/tmdbqt/ COMPONENT Devel
  )
endif()

# Install package config file
if(NOT WIN32)
if (Qt5Core_FOUND)
   #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libTmdbQt5.pc.cmake
   #${CMAKE_CURRENT_BINARY_DIR}/lib${TMDBQT_LIB_SONAME}.pc
   #)
   #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${TMDBQT_LIB_SONAME}.pc
   #DESTINATION ${LIB_DESTINATION}/pkgconfig
   #)
else()
   #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libtmdbqt.pc.cmake
   #${CMAKE_CURRENT_BINARY_DIR}/lib${TMDBQT_LIB_SONAME}.pc
   #)
   #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${TMDBQT_LIB_SONAME}.pc
   #DESTINATION ${LIB_DESTINATION}/pkgconfig
   #)
endif()
endif(NOT WIN32)

