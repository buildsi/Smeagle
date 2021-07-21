cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(SmeagleStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

include_directories(/opt/view/include)
link_directories(/opt/view/lib)

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

find_package(TBB REQUIRED)
target_include_directories(
  SmeagleStandalone
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}> ${Boost_INCLUDE_DIRS}
         ${include_dirs} ${DYNINST_INCLUDE_DIR} ${TBB_INCLUDE_DIRS}
)

target_link_libraries(SmeagleStandalone Smeagle::Smeagle cxxopts TBB::tbb)
