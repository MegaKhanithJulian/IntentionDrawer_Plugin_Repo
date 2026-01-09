# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 3 of the License, or (at your option) any later
# version.

#
# -------  Plugin setup --------
#
set(PKG_NAME IntentionDrawer)
set(PKG_VERSION "0.3.2")
set(PKG_PRERELEASE "beta") # Empty, or a tag like 'beta'

set(DISPLAY_NAME IMAGENIUS) # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME IntentionDrawer) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "The *BEST* plugin ever. For drawing intentions.")
set(PKG_DESCRIPTION
    [=[
I mean. In theory.
]=])

set(PKG_AUTHOR "Garschmek the Predator of Ice (HR Studemts)")
set(PKG_IS_OPEN_SOURCE "no")
set(PKG_HOMEPAGE https://www.youtube.com/watch?v=dQw4w9WgXcQ)
set(PKG_INFO_URL https://www.youtube.com/watch?v=dQw4w9WgXcQ)

option(WITH_TESTS "Whether or not to build the tests" OFF)
option(SANITIZE "What sanitizers to use" "")

if(NOT "${SANITIZE}" STREQUAL "OFF" AND NOT "${SANITIZE}" STREQUAL "")
  add_compile_options(-fsanitize=${SANITIZE} -fno-omit-frame-pointer)
  add_link_options(-fsanitize=${SANITIZE} -fno-omit-frame-pointer)
endif()

add_definitions(-DDASHBOARDSK_USE_SVG)
add_definitions(-DocpnUSE_GL)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_GUI_DESKTOP ${CMAKE_SOURCE_DIR}/src/dashboardskgui.cpp
                    ${CMAKE_SOURCE_DIR}/src/dashboardskguiimpl.cpp)
set(SRC_GUI_ANDROID ${CMAKE_SOURCE_DIR}/src/dashboardskguiandroid.cpp
                    ${CMAKE_SOURCE_DIR}/src/dashboardskguiimpl.cpp)

if(QT_ANDROID)
  set(SRC_GUI ${SRC_GUI_ANDROID})
else()
  set(SRC_GUI ${SRC_GUI_DESKTOP})
endif()

set(PKG_API_LIB api-20) # A dir in opencpn-libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers and
  # ocpn::api is available.

  # Fix OpenGL deprecated warnings in Xcode
  target_compile_definitions(${PACKAGE_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endmacro()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  if(WIN32)
    # add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/WindowsHeaders")
    # target_link_libraries(${PACKAGE_NAME} windows::headers)

    # add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/glu")
    # target_link_libraries(${PACKAGE_NAME} ocpn::glu_static)
  endif()
  if(LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GIOMM giomm-2.4)
    if(GIOMM_FOUND)
      include_directories(${GIOMM_INCLUDE_DIRS})
      target_link_libraries(${PACKAGE_NAME} ${GIOMM_LIBRARIES})
      add_definitions(-DDASHBOARDSK_USE_GIOMM)
    endif()
  endif()

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugin_dc")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugin-dc)

  add_subdirectory("opencpn-libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)
  if(${WITH_TESTS})
    include(CTest)
    add_subdirectory("${CMAKE_SOURCE_DIR}/tests")
    add_dependencies(${CMAKE_PROJECT_NAME} tests)
  endif()
endmacro()

add_custom_target(
  doxygen-docs
  COMMAND doxygen
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_target(
  asciidoxy-docs
  COMMAND asciidoxy -D apidocs --spec-file packages.toml api.adoc
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS doxygen-docs)
