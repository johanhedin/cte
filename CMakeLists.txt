cmake_minimum_required(VERSION 3.10)

# Define the project
project(tallium C)

# Import our local cmake Modules (if any)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

# We require a compiler with support for at least C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# If build type is not specified, it defaults to Debug. Set build type explicitly
# with -DCMAKE_BUILD_TYPE=<type> where <type> is one of: Debug, Release or RelWithDebInfo
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type defaulting to Debug. Change to Relese or RelWithDebInfo with -DCMAKE_BUILD_TYPE=<choise>")
    set(TALLIUM_TMP_BUILD_TYPE "Debug")
else()
   set(TALLIUM_TMP_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif ()
set(CMAKE_BUILD_TYPE "${TALLIUM_TMP_BUILD_TYPE}" CACHE STRING "Choose the type of build. Options are: Debug, Release or RelWithDebInfo" FORCE)

# Option for how to link to libtorium. Options are: Submodule, Static or Dynamic.
# Defaults to Submodule if not set
if(NOT DEFINED LIBTORIUM_LINK_TYPE)
    #message(STATUS "Defaulting to building libtorium as a static library. Change to shared with -DTORIUM_SHARED_LIBS=ON")
    message(STATUS "Linking to libtorium defaulting to Submodule. Change to Static or Dynamic with -DLIBTORIUM_LINK_TYPE=<linktype>")
    set(TOLIBTORIUM_LINK_TYPE_TMP "Submodule")
else()
    set(TOLIBTORIUM_LINK_TYPE_TMP ${LIBTORIUM_LINK_TYPE})
endif()
set(LIBTORIUM_LINK_TYPE ${TOLIBTORIUM_LINK_TYPE_TMP} CACHE STRING "Choose how to link to libtorium. Options are: Submodule, Static or Dynamic" FORCE)

# We need the PkgConfig cmake module to be able to find libraries we depend on
find_package(PkgConfig REQUIRED)

# Use GNUInstallDirs to get standard install directories for include, lib, bin and so on
include(GNUInstallDirs)

# Create a uninstall target if it does not already exist. We need this before
# any add_subdirectory so that uninstall targets from them do not propagate here
if(NOT TARGET uninstall)
    configure_file("${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Define a target for our program
add_executable(tallium src/tallium.c)

# Add compile and link options to our target
target_compile_options(tallium PRIVATE -Wall)
target_compile_options(tallium PRIVATE -Wextra)
target_compile_options(tallium PRIVATE -Wpedantic)
target_compile_options(tallium PRIVATE -fanalyzer)
target_compile_options(tallium PRIVATE -Wanalyzer-too-complex)
#target_compile_options(tallium PRIVATE -pg)
#target_link_options(tallium PRIVATE -pg)

# Add libtorium to our program depending on how to link
if(LIBTORIUM_LINK_TYPE STREQUAL "Submodule")
    set(TORIUM_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    mark_as_advanced(TORIUM_SHARED_LIBS)
    add_subdirectory(torium EXCLUDE_FROM_ALL)
    set(TORIUM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/torium/src)
    set(TORIUM_LIBRARY_DIRS "")
    set(TORIUM_LIBRARIES torium)
elseif(LIBTORIUM_LINK_TYPE STREQUAL "Static")
    pkg_check_modules(TORIUM REQUIRED libtorium-static>=1)
elseif(LIBTORIUM_LINK_TYPE STREQUAL "Dynamic")
    pkg_check_modules(TORIUM REQUIRED libtorium>=1)
else()
    message(FATAL_ERROR "LIBTORIUM_LINK_TYPE set to unknown value: ${LIBTORIUM_LINK_TYPE}")
endif()
target_include_directories(tallium PUBLIC ${TORIUM_INCLUDE_DIRS})
target_link_directories(tallium PUBLIC ${TORIUM_LIBRARY_DIRS})
target_link_libraries(tallium ${TORIUM_LIBRARIES})

# Add support for installing our program
install(TARGETS tallium RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

# Dump our config for review
message(STATUS "The following build options will be used for tallium:")
message(STATUS "    CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "    CMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "    CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "    BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
message(STATUS "    CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS "    CMAKE_INSTALL_FULL_INCLUDEDIR=${CMAKE_INSTALL_FULL_INCLUDEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LIBDIR=${CMAKE_INSTALL_FULL_LIBDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_BINDIR=${CMAKE_INSTALL_FULL_BINDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_SYSCONFDIR=${CMAKE_INSTALL_FULL_SYSCONFDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DATAROOTDIR=${CMAKE_INSTALL_FULL_DATAROOTDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LOCALSTATEDIR=${CMAKE_INSTALL_FULL_LOCALSTATEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DOCDIR=${CMAKE_INSTALL_FULL_DOCDIR}")

