AC_INIT(rtm-glib, 0.1.0)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AM_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_LIBTOOL
AC_ISC_POSIX
AC_HEADER_STDC

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])


#*******
# GNOME
#*******

GNOME_COMMON_INIT
GNOME_DEBUG_CHECK
GNOME_COMPILE_WARNINGS([maximum])
GNOME_MAINTAINER_MODE_DEFINES


#*************
# Dependencies
#*************

PKG_CHECK_MODULES([RTM_GLIB],
	[glib-2.0
	rest])

RTM_GLIB_LIBS="$RTM_GLIB_LIBS"
RTM_GLIB_CFLAGS="$RTM_GLIB_CFLAGS"

AC_SUBST([RTM_GLIB_LIBS])
AC_SUBST([RTM_GLIB_CFLAGS])


#***********
# Unit tests
#***********

PKG_CHECK_MODULES([CHECK],
	[check],
	[have_check="yes"],
	[have_check="no"])


#********
# gtk-doc
#********

GTK_DOC_CHECK([1.0])

GNOME_DOC_INIT


#*******
# Output
#*******

AC_CONFIG_FILES([
Makefile
rtm-glib/Makefile
tests/Makefile
examples/Makefile
doc/Makefile
doc/reference/Makefile
doc/reference/version.xml
])

AC_OUTPUT


#********
# Summary
#********

echo ""
echo "RTM-GLib $VERSION"
echo "=============="
echo ""
echo "   * Build API reference: ${enable_gtk_doc}"
echo "   * Check installed: ${have_check}"
if test "$have_check" = "yes"; then
echo "     Type \`make check' to run unit tests"
fi
echo ""
