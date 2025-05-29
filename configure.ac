#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.71])
AC_INIT([libmgeff],[1.5.3])
AC_CONFIG_SRCDIR(src/animation.c)
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([mgeffconfig.h])

MGEFF_NAME="mgeff"

dnl Set various version strings
#
# Making releases:
# Change the version, then:
#   _MICRO_VERSION += 1;
#   _INTERFACE_AGE += 1;
#   _BINARY_AGE += 1;
# if any functions have been added, set _INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set _BINARY_AGE and _INTERFACE_AGE to 0.
#
MGEFF_MAJOR_VERSION=1
MGEFF_MINOR_VERSION=5
MGEFF_MICRO_VERSION=3
MGEFF_INTERFACE_AGE=3
MGEFF_BINARY_AGE=3

MGEFF_VERSION=$MGEFF_MAJOR_VERSION.$MGEFF_MINOR_VERSION.$MGEFF_MICRO_VERSION

AC_SUBST(MGEFF_MAJOR_VERSION)
AC_SUBST(MGEFF_MINOR_VERSION)
AC_SUBST(MGEFF_MICRO_VERSION)
AC_SUBST(MGEFF_INTERFACE_AGE)
AC_SUBST(MGEFF_BINARY_AGE)
AC_SUBST(MGEFF_VERSION)

# libtool versioning
LT_RELEASE=$MGEFF_MAJOR_VERSION.$MGEFF_MINOR_VERSION
LT_CURRENT=`expr $MGEFF_MICRO_VERSION - $MGEFF_INTERFACE_AGE`
LT_REVISION=$MGEFF_INTERFACE_AGE
LT_AGE=`expr $MGEFF_BINARY_AGE - $MGEFF_INTERFACE_AGE`

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

dnl ========================================================================
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT

dnl ========================================================================
dnl Put the name and version of the package here

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT

AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([stdint.h stdlib.h string.h fcntl.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([bzero dup2 getcwd memmove memset putenv strchr strdup strerror strstr])

devel_mode="no"
AC_ARG_ENABLE(develmode,
[  --enable-develmode       developer mode <default=no>],
devel_mode=$enableval)

detail_debug="no"
AC_ARG_ENABLE(detail_debug,
[  --enable-detaildebug     detailed debug information <default=no>],
detail_debug=$enableval)

use_mgmesa="no"
AC_ARG_ENABLE(mgmesa,
[  --enable-mgmesa          enable Mesa for MiniGUI<default=no>],
use_mgmesa=$enableval)

use_mgegl="no"
AC_ARG_ENABLE(mgegl,
[  --enable-mgegl           enable eGL for MiniGUI<default=no>],
use_mgegl=$enableval)

use_opengles2="no"
AC_ARG_ENABLE(opengles2,
[  --enable-opengles2       enable OpenGL ES 2.0 <default=yes>],
use_opengles2=$enableval)

have_effectors="yes"
AC_ARG_ENABLE(effectors,
[  --enable-effectors       include effectors  <default=yes>],
have_effectors=$enableval)

effector_fastmode="yes"
AC_ARG_ENABLE(fastmode,
[  --enable-fastmode        enable fastmode <default=yes>],
effector_fastmode=$enableval)

effector_fivepointedstar="yes"
AC_ARG_ENABLE(efffivepointedstar,
[  --enable-efffivepointedstar      include five-pointed-star effector <default=yes>],
effector_fivepointedstar=$enableval)

effector_rotate="yes"
AC_ARG_ENABLE(effrotate,
[  --enable-effrotate       include rotate effector <default=yes>],
effector_rotate=$enableval)

effector_cubicrotate="yes"
AC_ARG_ENABLE(effcubicrotate,
[  --enable-effcubicrotate  include cubic-rotate effector <default=yes>],
effector_cubicrotate=$enableval)

effector_radarscan="yes"
AC_ARG_ENABLE(effradarscan,
[  --enable-effradarscan    include radar-scan effector <default=yes>],
effector_radarscan=$enableval)

effector_scroll="yes"
AC_ARG_ENABLE(effscroll,
[  --enable-effscroll       include scroll effector <default=yes>],
effector_scroll=$enableval)

effector_leafwindow="yes"
AC_ARG_ENABLE(effleafwindow,
[  --enable-effleafwindow   include leafwindow effector <default=yes>],
effector_leafwindow=$enableval)

effector_zoom="yes"
AC_ARG_ENABLE(effzoom,
[  --enable-effzoom         include zoom effector <default=yes>],
effector_zoom=$enableval)

effector_centersplit="yes"
AC_ARG_ENABLE(effcentersplit,
[  --enable-effcentersplit  include centersplit effector <default=yes>],
effector_centersplit=$enableval)

effector_flip="yes"
AC_ARG_ENABLE(effflip,
[  --enable-effflip         include flip effector <default=yes>],
effector_flip=$enableval)

effector_alpha="yes"
AC_ARG_ENABLE(effalpha,
[  --enable-effalpha        include alpha effector <default=yes>],
effector_alpha=$enableval)

effector_zip="yes"
AC_ARG_ENABLE(effzip,
[  --enable-effzip          include zip effector <default=yes>],
effector_zip=$enableval)

effector_clear="yes"
AC_ARG_ENABLE(effclear,
[  --enable-effclear        include clear effector <default=yes>],
effector_clear=$enableval)

effector_push="yes"
AC_ARG_ENABLE(effpush,
[  --enable-effpush         include push effector <default=yes>],
effector_push=$enableval)

effector_windowanimation="yes"
AC_ARG_ENABLE(windowanimation,
[  --enable-windowanimation support for window-animation <default=yes>],
effector_windowanimation=$enableval)

AC_ARG_WITH(libsuffix,
[  --with-libsuffix=ARG     configure the suffix of the library name.])

if test "x$with_libsuffix" = "x"; then
  LIB_SUFFIX=""
else
  LIB_SUFFIX="_$with_libsuffix"
fi

MGEFF_LIBNAME="${MGEFF_NAME}${LIB_SUFFIX}"

dnl ========================================================================
dnl check for installation of MiniGUI
dnl ========================================================================
PKG_CHECK_MODULES([MINIGUI], [minigui >= 3.2.0])

if test "x$devel_mode" = "xyes"; then
    have_effectors="yes"
    effector_fastmode="yes"
fi

if test "x$have_effectors" = "xyes"; then
    PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.5.0])
fi

if test "x$use_mgmesa" = "xyes"; then
    AC_DEFINE(HAVE_MESA_MINIGUI, 1, [Define if have Mesa for MiniGUI])
fi
if test "x$use_mgegl" = "xyes"; then
    AC_DEFINE(HAVE_EGL_SUPPORT, 1, [Define if have eGL for MiniGUI])
fi
if test "x$use_opengles2" = "xyes"; then
    AC_DEFINE(HAVE_GLES2_SUPPORT, 1, [Define if have OpenGL ES 2.0])
fi

EFFECTOR_SUBDIR=""
EFFECTOR_LIB=""
if test "x$have_effectors" = "xyes"; then
    AC_DEFINE(_MGEFF_EFFECTOR, 1, [Define if include effectors])
    EFFECTOR_SUBDIR="effector"
    EFFECTOR_LIB="effector/libeffectors.la"
fi

if test "x$effector_fastmode" = "xyes"; then
    AC_DEFINE(_MGEFF_FASTMODE, 1, [Define if enable fast mode of effectors])
fi
if test "x$effector_fivepointedstar" = "xyes"; then
    AC_DEFINE(_MGEFF_FIVEPOINTEDSTAREFFECTOR, 1, [Define if include five-pointed-star effector])
fi
if test "x$effector_rotate" = "xyes"; then
    AC_DEFINE(_MGEFF_ROTATEEFFECTOR, 1, [Define if include rotate effector])
fi
if test "x$effector_cubicrotate" = "xyes"; then
    AC_DEFINE(_MGEFF_CUBICROTATEEFFECTOR, 1, [Define if include cubic-rotate effector])
fi
if test "x$effector_radarscan" = "xyes"; then
    AC_DEFINE(_MGEFF_RADARSCANEFFECTOR, 1, [Define if include radar scan effector])
fi
if test "x$effector_scroll" = "xyes"; then
    AC_DEFINE(_MGEFF_SCROLLEFFECTOR, 1, [Define if include scroll effector])
fi
if test "x$effector_leafwindow" = "xyes"; then
    AC_DEFINE(_MGEFF_LEAFWINDOWEFFECTOR, 1, [Define if include leaf window effector])
fi
if test "x$effector_zoom" = "xyes"; then
    AC_DEFINE(_MGEFF_ZOOMEFFECTOR, 1, [Define if include zoom effector])
fi
if test "x$effector_centersplit" = "xyes"; then
    AC_DEFINE(_MGEFF_CENTERSPLITEFFECTOR, 1, [Define if include split effector])
fi
if test "x$effector_flip" = "xyes"; then
    AC_DEFINE(_MGEFF_FLIPEFFECTOR, 1, [Define if include flip effector])
fi
if test "x$effector_alpha" = "xyes"; then
    AC_DEFINE(_MGEFF_ALPHAEFFECTOR, 1, [Define if include alpha effector])
fi
if test "x$effector_zip" = "xyes"; then
    AC_DEFINE(_MGEFF_ZIPEFFECTOR, 1, [Define if include zip effector])
fi
if test "x$effector_clear" = "xyes"; then
    AC_DEFINE(_MGEFF_CLEAREFFECTOR, 1, [Define if include clear effector])
fi
if test "x$effector_push" = "xyes"; then
    AC_DEFINE(_MGEFF_PUSHEFFECTOR, 1, [Define if include push effector])
fi
if test "x$effector_windowanimation" = "xyes"; then
    AC_DEFINE(_MGEFF_WINDOWANIMATION, 1, [Define if include window animation])
fi

DEP_LIBS="$DEP_LIBS $MINIGUI_LIBS $MGPLUS_LIBS"
APP_LIBS="-l${MGEFF_LIBNAME} $DEP_LIBS"
PC_LIBS="$APP_LIBS"
PC_CFLAGS=""

dnl Check sqlite3 and xml2 for samples
AC_CHECK_LIB(sqlite3, sqlite3_open_v2, APP_LIBS="$APP_LIBS -lsqlite3", foo=bar)
AC_CHECK_LIB(xml2, xmlFree, APP_LIBS="$APP_LIBS -lxml2", foo=bar)

CFLAGS="$CFLAGS -D__MGEFF_LIB__"
if test "x$devel_mode" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -D_DEBUG"
    if test "x$detail_debug" = "xyes"; then
        CPPFLAGS="$CPPFLAGS -DDEBUG"
    fi

    if test "$ac_cv_c_compiler_gnu" = "yes"; then
        CPPFLAGS="$CPPFLAGS -Wall -Werror"
    fi
else
    CPPFLAGS="$CPPFLAGS -DNDEBUG"
fi

if test "$ac_cv_c_compiler_gnu" = "yes"; then
    CFLAGS="$CFLAGS -Wstrict-prototypes -pipe"
fi

AM_CONDITIONAL(MGEFF_NOSUFFIX, test "x$with_libsuffix" = "x")

AC_SUBST(EFFECTOR_SUBDIR)
AC_SUBST(EFFECTOR_LIB)
AC_SUBST(LIB_SUFFIX)
AC_SUBST(DEP_LIBS)
AC_SUBST(APP_LIBS)
AC_SUBST(PC_LIBS)
AC_SUBST(PC_CFLAGS)

AC_CONFIG_FILES([
    mgeff.pc
    Makefile
    m4/Makefile
    include/Makefile
    src/Makefile
    src/include/Makefile
    src/effector/Makefile
])
AC_OUTPUT

dnl samples/easing/Makefile
dnl samples/easing/src/Makefile
dnl echo
dnl echo "Creating include directory for samples"
dnl cd samples/
dnl if test ! -h mgeff; then
dnl    ln -s ../include mgeff
dnl fi
dnl cd ..
dnl echo "Done."
