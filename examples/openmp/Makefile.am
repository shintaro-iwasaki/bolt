# -*- Mode: Makefile; -*-
#
# See COPYRIGHT in top-level directory.
#

TESTS = \
	parallel_for_abt_task \
	parallel_for_abt_thread

check_PROGRAMS = $(TESTS)
noinst_PROGRAMS = $(TESTS)

include $(top_srcdir)/examples/Makefile.mk

parallel_for_abt_task_SOURCES = parallel_for_abt_task.c
parallel_for_abt_thread_SOURCES = parallel_for_abt_thread.c

testing:
	./parallel_for_abt_task
	./parallel_for_abt_thread
