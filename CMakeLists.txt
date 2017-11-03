cmake_minimum_required(VERSION 2.6)
project(mGEff)

set(MGEFF_MAJOR_VERSION 1)
set(MGEFF_MINOR_VERSION 0)
set(MGEFF_MICRO_VERSION 1)
set(MGEFF_LIBNAME "mgeff")

option(disable_shared "Disable shared library" OFF)
option(_MGEFF_EFFECTOR "Enable effector" ON)
if (_MGEFF_EFFECTOR)
    option(_MGEFF_FASTMODE "Enable fastmode effector" ON)
    option(HAVE_MESA_MINIGUI "Enable mesa-minigui support" OFF)
    option(HAVE_EGL_SUPPORT "Enable egl support" OFF)
    option(HAVE_GLES2_SUPPORT "Enable gles2 support" OFF)
    option(_MGEFF_FIVEPOINTEDSTAREFFECTOR "Enable effector-five-pointed-start" ON)
    option(_MGEFF_ROTATEEFFECTOR "Enable effector-rotate" ON)
    option(_MGEFF_RADARSCANEFFECTOR "Enable effector-radar-scan" ON)
    option(_MGEFF_CUBICROTATEEFFECTOR "Enable effector-cubic-rotate" ON)
    option(_MGEFF_SCROLLEFFECTOR "Enable effector-scroll" ON)
    option(_MGEFF_LEAFWINDOWEFFECTOR "Enable effector-leaf-window" ON)
    option(_MGEFF_ZOOMEFFECTOR "Enable effector-zoom" ON)
    option(_MGEFF_CENTERSPLITEFFECTOR "Enable effector-center-split" ON)
    option(_MGEFF_FLIPEFFECTOR "Enable effector-flip" ON)
    option(_MGEFF_ALPHAEFFECTOR "Enable effector-alpha" ON)
    option(_MGEFF_ZIPEFFECTOR "Enable effector-zip" ON)
    option(_MGEFF_CLEAREFFECTOR "Enable effector-clear" ON)
    option(_MGEFF_PUSHEFFECTOR "Enable effector-push" ON)
    option(_MGEFF_WINDOWANIMATION "Enable window animation support" ON)
endif (_MGEFF_EFFECTOR)

add_definitions(-D__MGEFF_LIB__)

INCLUDE (FindPkgConfig)

PKG_CHECK_MODULES (MINIGUI minigui)
IF (NOT MINIGUI_FOUND)
    MESSAGE(FATAL_ERROR 
       "MiniGUI is not properly installed on the system. You need 
        MiniGUI Ver 3.0.x or later for building this package. 
        Please configure and install MiniGUI Ver 3.0.x first."
        )
ENDIF (NOT MINIGUI_FOUND)
INCLUDE_DIRECTORIES (${MINIGUI_INCLUDE_DIRS})
LINK_DIRECTORIES (${MINIGUI_LIBRARY_DIRS})
#LIST (APPEND PC_LIBS_LIST ${MINIGUI_LIBRARIES})

IF (_MGEFF_EFFECTOR)
    PKG_CHECK_MODULES (MGPLUS mgplus)
    IF (NOT MGPLUS_FOUND)
        MESSAGE(FATAL_ERROR "mGPlus not found")
    ENDIF (NOT MGPLUS_FOUND)
    INCLUDE_DIRECTORIES (${MGPLUS_INCLUDE_DIRS})
    LINK_DIRECTORIES (${MGPLUS_LIBRARY_DIRS})
#    LIST (APPEND PC_LIBS_LIST ${MGPLUS_LIBRARIES})
ENDIF (_MGEFF_EFFECTOR)

LIST (APPEND PC_LIBS_LIST ${MGEFF_LIBNAME})
set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix "\${prefix}")
set (libdir "\${exec_prefix}/lib")
set (includedir "\${prefix}/include")
set (MGEFF_VERSION ${MGEFF_MAJOR_VERSION}.${MGEFF_MINOR_VERSION}.${MGEFF_MICRO_VERSION})

FOREACH (l ${PC_LIBS_LIST})
    SET (PC_LIBS "${PC_LIBS} -l${l}")
ENDFOREACH (l ${PC_LIBS_LIST})
configure_file(
    "${PROJECT_SOURCE_DIR}/mgeff.pc.in"
    "${PROJECT_BINARY_DIR}/mgeff.pc")
# pkgconfig
install (FILES "${PROJECT_BINARY_DIR}/mgeff.pc"
    DESTINATION lib/pkgconfig)

configure_file (
    "${PROJECT_SOURCE_DIR}/mgeffconfig.h.in"
    "${PROJECT_BINARY_DIR}/mgeffconfig.h"
    )
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(src)
add_subdirectory(include)

# Cmake Modules
install (DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/"
    DESTINATION share/minigui/cmake/Modules
    FILES_MATCHING PATTERN "*.cmake"
    PATTERN "*private*" EXCLUDE
    PATTERN ".svn" EXCLUDE
    )
