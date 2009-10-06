INCLUDES =			\
	-I$(top_srcdir)		\
	$(WARN_CFLAGS)		\
	$(RTM_GLIB_CFLAGS)

noinst_PROGRAMS = rtm-glib-example

rtm_glib_example_SOURCES =	\
	rtm-glib-example.c

rtm_glib_example_LDFLAGS =	\
	$(RTM_GLIB_LIBS)

rtm_glib_example_LDADD =			\
	$(top_builddir)/rtm-glib/librtm-glib.la
