SUBDIRS = rtm-glib tests examples

if ENABLE_GTK_DOC
SUBDIRS += doc
endif

pcfiles = rtm-glib.pc

rtm-glib.pc: rtm-glib.pc
	@cp -f $< $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = $(pcfiles)

EXTRA_DIST= \
	autoget.sh

DISTCHECK_CONFIGURE_FLAGS=--enable-gtk-doc

