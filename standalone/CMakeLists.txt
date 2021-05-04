cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(SmeagleStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.1
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME Smeagle SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(SmeagleStandalone ${sources})

set_target_properties(SmeagleStandalone PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "Smeagle")

target_link_libraries(SmeagleStandalone Smeagle::Smeagle cxxopts)
