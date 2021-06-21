cmake_minimum_required(VERSION 3.4.0 FATAL_ERROR)

# ---- Project ----

# This is the project's name and version
project(
  Smeagle
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

find_package(Dyninst REQUIRED)
find_package(Boost REQUIRED)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")
CPMAddPackage(
  NAME fmt
  GIT_TAG 7.1.3
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

# ---- Add source files ----
set(include_dirs smeagle/include source/parser)
set(sources source/corpora.cpp source/smeagle.cpp source/parser/x86_64/x86_64.cpp
            source/parser/ppc64le/ppc64le.cpp source/parser/aarch64/aarch64.cpp
)

# ---- Create library ----
add_library(Smeagle ${sources})

set_target_properties(Smeagle PROPERTIES CXX_STANDARD 17)

# Link dependencies
target_link_libraries(Smeagle PRIVATE fmt::fmt Boost::boost common symtabAPI)

target_include_directories(
  Smeagle
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}> ${Boost_INCLUDE_DIRS}
         ${include_dirs} ${DYNINST_INCLUDE_DIR}
)

target_include_directories(Smeagle PUBLIC ${DYNINST_INCLUDE_DIR})

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 7.1.3"
)
