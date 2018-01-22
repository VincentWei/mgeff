AM_CPPFLAGS=-I.. -Iinclude/

SUBDIRS=src

noinst_PROGRAMS=easing

easing_SOURCES=easing-main.c

easing_LDADD=../../src/libmgeff.la src/libeasing.la -lmgncs @APP_LIBS@

DIST_SUBDIRS=include res
