list(APPEND mgeff_source_files_ON
    animation.c
    animationgroup.c
    easing.c
    garbage-collection.c
    motioncurve.c
    schedular.c
    )

IF (WIN32)
    LIST (APPEND mgeff_source_files_ON eff-win32.c)
ENDIF (WIN32)

if (_MGEFF_EFFECTOR)

list(APPEND mgeff_source_files_${_MGEFF_WINDOWANIMATION}
    window-animation.c)

list(APPEND mgeff_source_files_${_MGEFF_EFFECTOR}
    effector/effector.c
    effector/effector-sink.c
    effector/effector-source.c
    effector/common-effector.c
    effector/3DCompute.c
    )

list(APPEND mgeff_source_files_${_MGEFF_LEAFWINDOWEFFECTOR}
    effector/leafwindow-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_ALPHAEFFECTOR}
    effector/alpha-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_FIVEPOINTEDSTAREFFECTOR}
    effector/star-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_RADARSCANEFFECTOR}
    effector/radarscan-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_SCROLLEFFECTOR}
    effector/scroll-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_ZOOMEFFECTOR}
    effector/zoom-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_FLIPEFFECTOR}
    effector/flip-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_ZIPEFFECTOR}
    effector/zip-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_CLEAREFFECTOR}
    effector/clear-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_PUSHEFFECTOR}
    effector/push-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_CUBICROTATEEFFECTOR}
    effector/cubic-effector.c
    )

list(APPEND mgeff_source_files_${_MGEFF_CENTERSPLITEFFECTOR}
    effector/centersplit-effector.c
    )

if (${HAVE_MESA_MINIGUI} OR ${HAVE_EGL_SUPPORT})

list(APPEND mgeff_source_files_${_MGEFF_EFFECTOR}
    effector/opengl-manager.c
    effector/gl-coverflow-effector.c)

list(APPEND mgeff_source_files_${_MGEFF_ROTATEEFFECTOR}
    effector/gl-rectrotate-effector.c
    )

list(APPEND mgeff_source_files_${HAVE_MESA_MINIGUI}
    effector/gl-cubeturn-effector.c
    )

endif (${HAVE_MESA_MINIGUI} OR ${HAVE_EGL_SUPPORT})

endif (_MGEFF_EFFECTOR)


#message(STATUS "source list:")
#foreach (_file ${mgeff_source_files_ON})
#    message(STATUS "  ${_file}")
#endforeach(_file)

include_directories(${PROJECT_SOURCE_DIR}/include include include/effector)

if (NOT disable_shared)
    # shared
    add_library(${MGEFF_LIBNAME} SHARED ${mgeff_source_files_ON})
else()
    # static
    add_library(${MGEFF_LIBNAME} STATIC ${mgeff_source_files_ON})
endif (NOT disable_shared)

set_target_properties(${MGEFF_LIBNAME} PROPERTIES VERSION
    ${MGEFF_MAJOR_VERSION}.${MGEFF_MINOR_VERSION}.${MGEFF_MICRO_VERSION})
set_target_properties (${MGEFF_LIBNAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
install (TARGETS ${MGEFF_LIBNAME}
    LIBRARY DESTINATION lib     # for .so  (dynamic library)
    ARCHIVE DESTINATION lib     # for .lib (import library)
    RUNTIME DESTINATION bin     # for .dll (dynamic library)
    )
