ACLOCAL_AMFLAGS=-I m4
SUBDIRS=m4 src include

# header files
libmgeffincludedir=$(includedir)/mgeff
libmgeffinclude_HEADERS=mgeffconfig.h

# pkgconfig files
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=mgeff.pc

EXTRA_DIST=INSTALL README.md LICENSE

docs:
	doxygen Doxyfile
