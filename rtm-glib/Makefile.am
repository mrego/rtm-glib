INCLUDES =			\
	-I$(top_srcdir)		\
	$(WARN_CFLAGS)		\
	$(RTM_GLIB_CFLAGS)

lib_LTLIBRARIES = librtm-glib.la

librtm_glib_la_SOURCES =	\
	rtm-glib.h		\
	rtm-glib.c		\
	rtm-list.h		\
	rtm-list.c		\
	rtm-task.h		\
	rtm-task.c		\
	rtm-error.h		\
	rtm-error.c		\
	rtm-util.h		\
	rtm-util.c		\
	rtm-location.h		\
	rtm-location.c		\
	rtm-time-zone.h		\
	rtm-time-zone.c

librtm_glib_la_LDFLAGS =	\
	$(RTM_GLIB_LIBS)
