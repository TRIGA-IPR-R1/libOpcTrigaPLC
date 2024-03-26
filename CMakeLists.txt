cmake_minimum_required(VERSION 3.12)

include(FetchContent)

project(
  libOpcTrigaPLC
  VERSION 0.0.0
  DESCRIPTION "Comunicate with the PLC in the Triga Control Table using OPC"
  HOMEPAGE_URL "https://github.com/SEURT-CDTN/libOpcTrigaPLC"
  LANGUAGES CXX)

FetchContent_Declare(
  open62541pp
  GIT_REPOSITORY https://github.com/open62541pp/open62541pp
  GIT_TAG master)

FetchContent_GetProperties(open62541pp)
if(NOT open62541pp_POPULATED)
  FetchContent_Populate(open62541pp)
  add_subdirectory(${open62541pp_SOURCE_DIR} ${open62541pp_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

set(LIBOPCTRIGAPLC_SRC src/libOpcTrigaPLC.cpp)

add_library(opcTrigaPLC ${LIBOPCTRIGAPLC_SRC})
add_library(opcTrigaPLC::opcTrigaPLC ALIAS opcTrigaPLC)

target_include_directories(opcTrigaPLC PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(opcTrigaPLC PUBLIC open62541pp)

add_executable(test src/test.cpp)
target_include_directories(test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test PRIVATE opcTrigaPLC)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/libOpcTrigaPLC/)
install(
  TARGETS opcTrigaPLC
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${open62541pp_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/open62541pp/)
install(
  TARGETS open62541pp
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${open62541pp_SOURCE_DIR}/3rdparty/open62541/include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/open62541/)
install(
  TARGETS open62541
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(TARGETS test RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
