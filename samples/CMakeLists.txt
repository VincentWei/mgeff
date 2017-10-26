cmake_minimum_required(VERSION 2.6)
project(mgeff-samples)

#find_package(mgeff REQUIRED)
#set(mgeff_sample_libraries ${mgeff_LIBRARIES})
if (WIN32)

find_path(path_mgeff_h mgeff/mgeff.h)
if (NOT path_mgeff_h)
    message(FATAL_ERROR "mGEff header files not found")
endif (NOT path_mgeff_h)
include_directories(AFTER ${path_mgeff_h})

#find_library(mgeff_library_path mgeff.lib)
#if (NOT mgeff_library_path)
#    message(FATAL_ERROR "mGEff library file not found")
#endif(NOT mgeff_library_path)
#message(STATUS "mgeff_library_path=${mgeff_library_path}")
#link_directories(${mgeff_library_path})
link_directories("${path_mgeff_h}/../lib")

set(mgeff_sample_libraries libminigui_sa libmgplus libjpeg libpng mgeff)

else (WIN32) # not WIN32

set(mgeff_sample_libraries minigui_ths mgplus jpeg png pthread z freetype mgeff m)

endif (WIN32)

set (CMAKE_MODULE_PATH
    "${CMAKE_MODULE_PATH}"
    "${CMAKE_INSTALL_PREFIX}/share/minigui/cmake/Modules"
    )
find_package (MiniGUI)

if (NOT MINIGUI_FOUND)
   message(FATAL_ERROR "MiniGUI not found")
endif (NOT MINIGUI_FOUND)

include_directories(AFTER ${MINIGUI_INCLUDE_DIRS})
link_directories(${MINIGUI_LIBRARY_DIRS})

link_libraries(${mgeff_sample_libraries})

#add_subdirectory(anim-wnd)
add_subdirectory(animation)

add_subdirectory(ball)
add_subdirectory(banner)
add_subdirectory(btnshow)
add_subdirectory(curve)
add_subdirectory(custom-effector)
#add_subdirectory(easing)
add_subdirectory(effector)
add_subdirectory(image-box)
add_subdirectory(particle-sys)
