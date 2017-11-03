cmake_minimum_required(VERSION 2.6)
project(mgeff-samples)

##################### Dependencies #######################
INCLUDE (FindPkgConfig)
PKG_CHECK_MODULES (MINIGUI minigui)
PKG_CHECK_MODULES (MGPLUS mgplus)
PKG_CHECK_MODULES (MGEFF mgeff)
SET (APP_LIBS ${MGEFF_LIBRARIES} ${MGPLUS_LIBRARIES} ${MINIGUI_LIBRARIES})

include_directories(AFTER ${MINIGUI_INCLUDE_DIRS})
link_directories(${MINIGUI_LIBRARY_DIRS})
link_libraries(${APP_LIBS})

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
