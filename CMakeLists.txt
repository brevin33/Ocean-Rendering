# CMakeList.txt : CMake project for Vulkan-CMake-Template, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Vulkan-CMake-Template")




find_package(imgui CONFIG REQUIRED)

add_subdirectory(include/dep/MesmerizeRenderer)


add_executable(OceanRendering "src/main.cpp" "include/vertex.h" "src/ocean.cpp" "include/ocean.h" "include/camera.h" "include/stb_image.h")

target_include_directories(OceanRendering PUBLIC include/private)
target_include_directories(OceanRendering PUBLIC include)

target_precompile_headers(OceanRendering PRIVATE include/pch.h)

target_link_libraries(OceanRendering PUBLIC imgui::imgui)
target_link_libraries(OceanRendering PUBLIC MesmerizeRenderer)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OceanRendering PROPERTY CXX_STANDARD 20)
endif()
