# - This module determines the mgeff library of the system
# The following variables are set if the library found:
# MGEFF_FOUND - If false do nnt try to use mgeff.
# MGEFF_INCLUDE_DIR - where to find the headfile of library.
# MGEFF_LIBRARY_DIR - where to find the mgeff library.
# MGEFF_LIBRARY - the library needed to use mgeff.

# find the headfile of library

set (MGEFF_HEADS mgeff/mgeff.h)
find_path (MGEFF_INCLUDE_DIR ${MGEFF_HEADS})

set (MGEFF_NAMES mgeff libmgeff)
find_library (MGEFF_LIBRARY NAMES ${MGEFF_NAMES})

# just find one of dependency, guess other one.
if (NOT MGEFF_LIBRARY AND MGEFF_INCLUDE_DIR)
	message ("We just find the headfile, try to guess the library location.")
	set (MGEFF_LIBRARY_DIR "${MGEFF_INCLUDE_DIR}/../lib")
	find_library (MGEFF_LIBRARY NAMES ${MGEFF_NAMES} PATHS ${MGEFF_LIBRARY_DIR})
elseif (NOT MGEFF_INCLUDE_DIR AND MGEFF_LIBRARY)
	message ("We just find the lib file, try to guess the include location.")
	string (REGEX REPLACE "[a-z|A-Z|0-9|_|-]+[.].*$" "" MGEFF_LIBRARY_DIR ${MGEFF_LIBRARY})
	find_path (MGEFF_INCLUDE_DIR ${MGEFF_HEADS} "${MGEFF_LIBRARY_DIR}../included")
endif()

# find the library.
if (MGEFF_INCLUDE_DIR AND MGEFF_LIBRARY)
	set (MGEFF_FOUND TRUE)
	if (NOT MGEFF_LIBRARY_DIR)
		string (REGEX REPLACE "[a-z|A-Z|0-9|_|-]+[.].*$" "" MGEFF_LIBRARY_DIR ${MGEFF_LIBRARY})
	endif ()
    string (REGEX MATCH "[a-z|A-Z|0-9|_|-]+[.](a|so|lib|dll|LIB|DLL)[^/]*$" MGEFF_LIBRARY_WHOLE_NAME "${MGEFF_LIBRARY}")
    string (REGEX MATCH "[^(lib)][a-z|A-Z|0-9|_|-]+" MGEFF_LIBRARIES ${MGEFF_LIBRARY_WHOLE_NAME})
	message (STATUS "Find MGEFF include in ${MGEFF_INCLUDE_DIR}")
	message (STATUS "Find MGEFF library in ${MGEFF_LIBRARY}")
else ()
	message (STATUS "Could NOT find MGEFF, (missing: MGEFF_INCLUDE_DIR MGEFF_LIBRARY)")
endif ()

