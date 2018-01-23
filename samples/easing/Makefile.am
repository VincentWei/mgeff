AM_CPPFLAGS=-I.. -Iinclude/

SUBDIRS=src

noinst_PROGRAMS=easing

easing_SOURCES=easing-main.c 
easing_LDADD=src/.libs/libeasing.al ../../src/.libs/libmgeff.a -lmgncs -lmgutils @APP_LIBS@

DIST_SUBDIRS=include res
