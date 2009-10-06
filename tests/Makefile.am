TESTS =				\
	check-rtm-list		\
	check-rtm-task


check_PROGRAMS =		\
	check-rtm-list		\
	check-rtm-task


check_rtm_list_SOURCES =	\
	check-rtm-list.c

check_rtm_task_SOURCES =	\
	check-rtm-task.c


INCLUDES =			\
	@CHECK_CFLAGS@		\
	$(RTM_GLIB_CFLAGS)

LIBS =						\
	@CHECK_LIBS@				\
	$(RTM_GLIB_LIBS)			\
	$(top_builddir)/rtm-glib/librtm-glib.la
