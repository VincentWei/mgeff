AM_CPPFLAGS=-I.. -Iinclude/

SUBDIRS=src

noinst_PROGRAMS=easing

easing_SOURCES=easing-main.c 
easing_LDADD=src/libeasing.la ../../src/libmgeff.la -lmgncs -lmgutils @APP_LIBS@

DIST_SUBDIRS=include res
