/*
 * kmp_abt_omp.c -- implementation of OpenMP functions.
 */


//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


#include "omp.h"        /* extern "C" declarations of user-visible routines */
#include "kmp_abt.h"

/* OpenMP 4.1 */

/* set API functions */
void
omp_set_num_threads(int num_threads)
{
    __kmp_set_num_threads(num_threads, __kmp_entry_thread());
}

void
omp_set_dynamic(int dynamic_threads)
{
    assert(0);
}

void
omp_set_nested(int nested)
{
    assert(0);
}

void
omp_set_max_active_levels(int max_levels)
{
    assert(0);
}

void
omp_set_schedule(omp_sched_t kind, int chunk_size)
{
    assert(0);
}


/* query API functions */
int
omp_get_num_threads(void)
{
    assert(0);
    return 0;
}

int
omp_get_dynamic(void)
{
    assert(0);
    return 0;
}

int
omp_get_nested(void)
{
    assert(0);
    return 0;
}

int
omp_get_max_threads(void)
{
    assert(0);
    return 0;
}

int
omp_get_thread_num(void)
{
    assert(0);
    return 0;
}

int
omp_get_num_procs(void)
{
    assert(0);
    return 0;
}

int
omp_in_parallel(void)
{
    assert(0);
    return 0;
}

int
omp_in_final(void)
{
    assert(0);
    return 0;
}

int
omp_get_active_level(void)
{
    assert(0);
    return 0;
}

int
omp_get_level(void)
{
    assert(0);
    return 0;
}

int
omp_get_ancestor_thread_num(int level)
{
    assert(0);
    return 0;
}

int
omp_get_team_size(int level)
{
    assert(0);
    return 0;
}

int
omp_get_thread_limit(void)
{
    assert(0);
    return 0;
}

int
omp_get_max_active_levels(void)
{
    assert(0);
    return 0;
}

void
omp_get_schedule(omp_sched_t *kind, int *chunk_size)
{
    assert(0);
}

int
omp_get_max_task_priority(void)
{
    assert(0);
    return 0;
}


/* lock API functions */
void
omp_init_lock(omp_lock_t *lock)
{
    assert(0);
}

void
omp_set_lock(omp_lock_t *lock)
{
    assert(0);
}

void
omp_unset_lock(omp_lock_t *lock)
{
    assert(0);
}

void
omp_destroy_lock(omp_lock_t *lock)
{
    assert(0);
}

int
omp_test_lock(omp_lock_t *lock)
{
    assert(0);
    return 0;
}


/* nested lock API functions */
void
omp_init_nest_lock(omp_nest_lock_t *lock)
{
    assert(0);
}

void
omp_set_nest_lock(omp_nest_lock_t *lock)
{
    assert(0);
}

void
omp_unset_nest_lock(omp_nest_lock_t *lock)
{
    assert(0);
}

void
omp_destroy_nest_lock(omp_nest_lock_t *lock)
{
    assert(0);
}

int
omp_test_nest_lock(omp_nest_lock_t *lock)
{
    assert(0);
    return 0;
}


/* hinted lock initializers */
void
omp_init_lock_with_hint(omp_lock_t *lock, omp_lock_hint_t hint)
{
    assert(0);
}

void
omp_init_nest_lock_with_hint(omp_nest_lock_t *lock, omp_lock_hint_t hint)
{
    assert(0);
}


/* time API functions */
double
omp_get_wtime(void)
{
    assert(0);
    return 0.0;
}

double
omp_get_wtick(void)
{
    assert(0);
    return 0.0;
}


/* OpenMP 4.0 */
int
omp_get_default_device(void)
{
    assert(0);
    return 0;
}

void
omp_set_default_device(int)
{
    assert(0);
}

int
omp_is_initial_device(void)
{
    assert(0);
    return 0;
}

int
omp_get_num_devices(void)
{
    assert(0);
    return 0;
}

int
omp_get_num_teams(void)
{
    assert(0);
    return 0;
}

int
omp_get_team_num(void)
{
    assert(0);
    return 0;
}

int
omp_get_cancellation(void)
{
    assert(0);
    return 0;
}


/* OpenMP 4.0 affinity API */
omp_proc_bind_t
omp_get_proc_bind(void)
{
    assert(0);
    return omp_proc_bind_false;
}


/* OpenMP 4.5 affinity API */
int
omp_get_num_places(void)
{
    assert(0);
    return 0;
}

int
omp_get_place_num_procs(int)
{
    assert(0);
    return 0;
}

void
omp_get_place_proc_ids(int place_num, int *ids)
{
    assert(0);
}

int
omp_get_place_num(void)
{
    assert(0);
    return 0;
}

int
omp_get_partition_num_places(void)
{
    assert(0);
    return 0;
}

void
omp_get_partition_place_nums(int *place_nums)
{
    assert(0);
}


