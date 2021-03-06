// RUN: %libomp-compile-and-run
// REQUIRES: abt
#include "omp_testsuite.h"
#include <string.h>
#include <stdio.h>

int test_task_untied_threadid2(int num_threads) {
  int i, vals[NUM_TASKS];
  ABT_thread abt_threads[NUM_TASKS];
  memset(vals, 0, sizeof(vals));

  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp master
    {
      for (i = 0; i < NUM_TASKS; i++) {
        #pragma omp task firstprivate(i) untied
        {
          ABT_EXIT_IF_FAIL(ABT_thread_self(&abt_threads[i]));

          // Context switching in OpenMP.
          #pragma omp taskyield

          int omp_thread_id2 = omp_get_thread_num();
          ABT_thread abt_thread = abt_threads[i];
          ABT_thread abt_thread2;
          ABT_EXIT_IF_FAIL(ABT_thread_self(&abt_thread2));
          ABT_bool abt_thread_equal;
          ABT_EXIT_IF_FAIL(ABT_thread_equal(abt_thread, abt_thread2,
                                            &abt_thread_equal));
          if (abt_thread_equal == ABT_TRUE) {
            vals[i] += 1;
          }

          // Context switching in Argobots.
          ABT_EXIT_IF_FAIL(ABT_thread_yield());

          int omp_thread_id3 = omp_get_thread_num();
          if (omp_thread_id2 == omp_thread_id3) {
            // Argobots context switch does not change the thread-task mapping.
            vals[i] += 2;
          }
        }
      }
    }
  }

  for (i = 0; i < NUM_TASKS; i++) {
    if (vals[i] != 3) {
      printf("vals[%d] == %d\n", i, vals[i]);
      return 0;
    }
  }
  return 1;
}

int main() {
  int i, num_failed = 0;
  for (i = 0; i < REPETITIONS; i++) {
    if (!test_task_untied_threadid2(i + 1)) {
      num_failed++;
    }
  }
  return num_failed;
}
