# -*- Mode: Makefile; -*-
#
# See COPYRIGHT in top-level directory.
#

TESTS = \
	parallel_for_abt_task \
	parallel_for_abt_thread \
	nested_parallel_for_abt_task \
	nested_parallel_for_abt_thread \
	nested_parallel_for_block_abt_thread \
	nested_parallel_for_block_abt_task \
	nested_parallel_for_irregular_abt_task \
	nested_parallel_for_irregular_abt_thread \
	task_single_producer_abt_task \
	task_single_producer_abt_thread \
	task_multiple_producer_abt_task \
	task_multiple_producer_abt_thread \
	task_nested_abt_task \
	task_nested_lvl2_abt_task

check_PROGRAMS = $(TESTS)
noinst_PROGRAMS = $(TESTS)

include $(top_srcdir)/examples/Makefile.mk

parallel_for_abt_task_SOURCES = parallel_for_abt_task.c
parallel_for_abt_thread_SOURCES = parallel_for_abt_thread.c
nested_parallel_for_abt_task_SOURCES = nested_parallel_for_abt_task.c
nested_parallel_for_abt_thread_SOURCES = nested_parallel_for_abt_thread.c
nested_parallel_for_block_abt_thread_SOURCES = nested_parallel_for_block_abt_thread.c
nested_parallel_for_block_abt_task_SOURCES = nested_parallel_for_block_abt_task.c
nested_parallel_for_irregular_abt_task_SOURCES = nested_parallel_for_irregular_abt_task.c
nested_parallel_for_irregular_abt_thread_SOURCES = nested_parallel_for_irregular_abt_thread.c
task_single_producer_abt_task_SOURCES = task_single_producer_abt_task.c
task_single_producer_abt_thread_SOURCES = task_single_producer_abt_thread.c
task_multiple_producer_abt_task_SOURCES = task_multiple_producer_abt_task.c
task_multiple_producer_abt_thread_SOURCES = task_multiple_producer_abt_thread.c
task_nested_abt_task_SOURCES = task_nested_abt_task.c
task_nested_lvl2_abt_task_SOURCES = task_nested_lvl2_abt_task.c

testing:
	./parallel_for_abt_task
	./parallel_for_abt_thread
	./nested_parallel_for_abt_task
	./nested_parallel_for_abt_thread
	./nested_parallel_for_block_abt_thread
	./nested_parallel_for_block_abt_task
	./nested_parallel_for_irregular_abt_task
	./nested_parallel_for_irregular_abt_thread
	./task_single_producer_abt_task
	./task_single_producer_abt_thread
	./task_multiple_producer_abt_task
	./task_multiple_producer_abt_thread
	./task_nested_abt_task
	./task_nested_lvl2_abt_task
