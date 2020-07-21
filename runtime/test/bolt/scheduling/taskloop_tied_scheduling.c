// RUN: %libomp-compile && env KMP_ABT_NUM_ESS=4 %libomp-run
// REQUIRES: abt
#include "omp_testsuite.h"
#include "bolt_scheduling_util.h"

int test_taskloop_tied_scheduling() {
  int i, vals[6];
  memset(vals, 0, sizeof(int) * 6);

  timeout_barrier_t barrier;
  timeout_barrier_init(&barrier);

  #pragma omp parallel num_threads(4)
  {
    if (omp_get_thread_num() >= 2) {
      timeout_barrier_wait(&barrier, 4);
    }
    // 6 barrier_waits in tasks and 2 barrier_waits in threads
    #pragma omp master
    {
      check_num_ess(4);
      #pragma omp taskloop grainsize(1)
      for (i = 0; i < 6; i++) {
        timeout_barrier_wait(&barrier, 4);
        vals[i] = 1;
      }
    }
  }

  for (i = 0; i < 6; i++) {
    if (vals[i] != 1) {
      printf("vals[%d] == %d\n", i, vals[i]);
      return 0;
    }
  }
  return 1;
}

int main() {
  int i, num_failed = 0;
  for (i = 0; i < REPETITIONS; i++) {
    if (!test_taskloop_tied_scheduling()) {
      num_failed++;
    }
  }
  return num_failed;
}
