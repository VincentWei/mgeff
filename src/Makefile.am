AM_CPPFLAGS=-I../include -Iinclude/

SUBDIRS=include effector

lib_LTLIBRARIES=libmgeff@LIB_SUFFIX@.la

COMMON_LADD=effector/libeffectors.la
COMMON_SRCS=animation.c animationgroup.c easing.c garbage-collection.c motioncurve.c schedular.c window-animation.c 

libmgeff@LIB_SUFFIX@_la_LIBADD=$(COMMON_LADD)
libmgeff@LIB_SUFFIX@_la_SOURCES=$(COMMON_SRCS)
libmgeff@LIB_SUFFIX@_la_LDFLAGS=-release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

EXTRA_DIST=eff-win32.c 
