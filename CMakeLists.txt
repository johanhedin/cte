cmake_minimum_required(VERSION 2.8.12...3.10)

# Define the project
project(cte C)

# Import our local cmake Modules (if any)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

# We require a compiler with support for at least C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# If build type is not specified, it defaults to Debug. Set build type explicitly
# with -DCMAKE_BUILD_TYPE=<type> where <type> is one of: Debug, Release or RelWithDebInfo
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type defaulting to Debug. Change to Relese or RelWithDebInfo with -DCMAKE_BUILD_TYPE=<choise>")
    set(CTE_TMP_BUILD_TYPE "Debug")
else()
   set(CTE_TMP_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif ()
set(CMAKE_BUILD_TYPE "${CTE_TMP_BUILD_TYPE}" CACHE STRING "Choose the type of build. Options are: Debug, Release or RelWithDebInfo" FORCE)

# Option for how to link to libctl. Options are: Submodule, Static or Shared.
# Defaults to Submodule if not set
if(NOT DEFINED CTE_LIBCTL_LINK_MODE)
    #message(STATUS "Defaulting to building libctl as a static library. Change to shared with -DTORIUM_SHARED_LIBS=ON")
    message(STATUS "Linking to libctl defaulting to Submodule. Change to Static or Shared with -DCTE_LIBCTL_LINK_MODE=<choise>")
    set(CTE_LIBCTL_LINK_MODE_TMP "Submodule")
else()
    set(CTE_LIBCTL_LINK_MODE_TMP ${CTE_LIBCTL_LINK_MODE})
endif()
set(CTE_LIBCTL_LINK_MODE ${CTE_LIBCTL_LINK_MODE_TMP} CACHE STRING "Choose how to link to libctl. Options are: Submodule, Static or Shared" FORCE)

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
add_executable(cte src/cte.c)

# Add compile and link options to our target. We differentiate between gcc and clang
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_VERSION VERSION_LESS "3.1")
        target_compile_options(cte PRIVATE -std=gnu${CMAKE_C_STANDARD})
    endif()
    target_compile_options(cte PRIVATE -Wall)
    target_compile_options(cte PRIVATE -Wextra)
    target_compile_options(cte PRIVATE -pedantic)
    #target_compile_options(cte PRIVATE -pg)
    #target_link_options(cte PRIVATE -pg)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_VERSION VERSION_LESS "3.1")
        target_compile_options(cte PRIVATE -std=gnu${CMAKE_C_STANDARD})
    endif()
    target_compile_options(cte PRIVATE -Wall)
    target_compile_options(cte PRIVATE -Wextra)
    target_compile_options(cte PRIVATE -Wpedantic)
else()
endif()

# Add libctl to our program depending on how to link
if(CTE_LIBCTL_LINK_MODE STREQUAL "Submodule")
    set(CTL_LINK_TYPE "Intree" CACHE STRING "" FORCE)
    mark_as_advanced(CTL_LINK_TYPE)
    add_subdirectory(ctl EXCLUDE_FROM_ALL)
    set(CTL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ctl/include)
    set(CTL_LIBRARY_DIRS "")
    set(CTL_LIBRARIES ctl)
elseif(CTE_LIBCTL_LINK_MODE STREQUAL "Static")
    pkg_check_modules(CTL REQUIRED libctl-static>=1)
elseif(CTE_LIBCTL_LINK_MODE STREQUAL "Shared")
    pkg_check_modules(CTL REQUIRED libctl>=1)
else()
    message(FATAL_ERROR "CTE_LIBCTL_LINK_MODE set to unknown value: ${CTE_LIBCTL_LINK_MODE}")
endif()
target_include_directories(cte PUBLIC ${CTL_INCLUDE_DIRS})
target_link_directories(cte PUBLIC ${CTL_LIBRARY_DIRS})
target_link_libraries(cte ${CTL_LIBRARIES})

# Add support for installing our program
install(TARGETS cte RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

# Dump our config for review
message(STATUS "The following build options will be used for cte:")
message(STATUS "    CTE_LIBCTL_LINK_MODE=${CTE_LIBCTL_LINK_MODE}")
message(STATUS "    CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "    CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID}")
message(STATUS "    CMAKE_C_COMPILER_VERSION=${CMAKE_C_COMPILER_VERSION}")
message(STATUS "    CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS "    CMAKE_INSTALL_FULL_INCLUDEDIR=${CMAKE_INSTALL_FULL_INCLUDEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LIBDIR=${CMAKE_INSTALL_FULL_LIBDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_BINDIR=${CMAKE_INSTALL_FULL_BINDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_SYSCONFDIR=${CMAKE_INSTALL_FULL_SYSCONFDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DATAROOTDIR=${CMAKE_INSTALL_FULL_DATAROOTDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LOCALSTATEDIR=${CMAKE_INSTALL_FULL_LOCALSTATEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DOCDIR=${CMAKE_INSTALL_FULL_DOCDIR}")
