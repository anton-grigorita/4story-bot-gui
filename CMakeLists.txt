cmake_minimum_required(VERSION 3.23)
project(gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/$<CONFIG>)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(MSVC)
  add_compile_options(/utf-8 /W3 /permissive- /sdl)
endif()

# path
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(IMGUI_DIR ${SRC_DIR}/lib/imgui)
set(LAYOUT_DIR ${SRC_DIR}/layout)
set(RES_DIR ${SRC_DIR}/resource)
set(INJECTOR_DIR ${SRC_DIR}/lib/injector)
set(SPDLOG_DIR ${SRC_DIR}/lib/spdlog)

set(SOURCES
  ${LAYOUT_DIR}/info-tab/log-viewer/logViewer.cpp
  ${LAYOUT_DIR}/character-tab/characterContainer.cpp
  ${LAYOUT_DIR}/character-tab/mageKeybindsContainer/mageKeybindsContainer.cpp
  ${LAYOUT_DIR}/character-tab/warriorKeybindsContainer/warriorKeybindsContainer.cpp
  ${LAYOUT_DIR}/container.cpp
  ${LAYOUT_DIR}/farm-spot-tab/farmSpotContainer.cpp
  ${LAYOUT_DIR}/help-tab/helpContainer.cpp
  ${LAYOUT_DIR}/info-tab/infoContainer.cpp
  ${LAYOUT_DIR}/settings-tab/settingsContainer.cpp
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_impl_dx11.cpp
  ${IMGUI_DIR}/imgui_impl_win32.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${SRC_DIR}/main.cpp
  ${RES_DIR}/gui.rc
)

add_executable(4SBot WIN32 ${SOURCES})

target_include_directories(4SBot PRIVATE
  ${SRC_DIR}/lib
  ${IMGUI_DIR}
  ${IMGUI_DIR}/include
  ${INJECTOR_DIR}/include
  ${SPDLOG_DIR}/include
  ${LAYOUT_DIR}
  ${LAYOUT_DIR}/character-tab
  ${LAYOUT_DIR}/character-tab/mageKeybindsContainer
  ${LAYOUT_DIR}/character-tab/warriorKeybindsContainer
  ${LAYOUT_DIR}/farm-spot-tab
  ${LAYOUT_DIR}/help-tab
  ${LAYOUT_DIR}/info-tab
  ${LAYOUT_DIR}/info-tab/log-viewer
  ${LAYOUT_DIR}/settings-tab
  ${RES_DIR}
)

add_library(injectorlib STATIC IMPORTED GLOBAL)
set_target_properties(injectorlib PROPERTIES
  IMPORTED_LOCATION "${INJECTOR_DIR}/injector.lib"
)
target_link_libraries(4SBot PRIVATE d3d11 d3dcompiler dxgi injectorlib)

if(MSVC)
  target_link_options(4SBot PRIVATE
    "/MANIFESTUAC:level='requireAdministrator'"
    $<$<CONFIG:Release>:/LTCG>
  )
endif()

if(WIN32 AND DEFINED CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
  message(STATUS "Windows SDK: ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
endif()
