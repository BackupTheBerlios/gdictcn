dnl Process this file with autoconf to produce a configure script.

AC_INIT(src/main.c)
AM_CONFIG_HEADER(config.h)

PACKAGE=gdictcn
VERSION=0.1

AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_INSTALL

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.4 glib-2.0 >= 2.4)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
AC_SUBST(GTK_XIM_FLAGS)

AC_PATH_PROG(CURLCFG, curl-config, [no])
if test "$CURLCFG" = "no"; then
	AC_MSG_ERROR([Soryy, can't found curl library])
fi
CURL_CFLAGS=`$CURLCFG --cflags`
CURL_LIBS=`$CURLCFG --libs`
AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_LIBS)

AC_PATH_PROG(XML2CFG, xml2-config, [no])
if test "$XML2CFG" = "no"; then
	AC_MSG_ERROR([Soryy, can't found curl library])
fi
XML2_CFLAGS=`$XML2CFG --cflags`
XML2_LIBS=`$XML2CFG --libs`
AC_SUBST(XML2_CFLAGS)
AC_SUBST(XML2_LIBS)

ALL_LINGUAS="zh_CN"
AM_GLIB_GNU_GETTEXT

GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["${GETTEXT_PACKAGE}"],[gettext domain])

ALL_LINGUAS="zh_CN"

AC_OUTPUT([
Makefile 
src/Makefile 
pixmaps/Makefile 
po/Makefile.in
])
