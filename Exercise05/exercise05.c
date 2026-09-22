#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib_serial(int n) {
    if (n < 2) return n;
    return fib_serial(n - 1) + fib_serial(n - 2);
}

int fib_task(int n) {
    int i, j;
    if (n < 2) return n;
    // Cutoff threshold for small subproblems to avoid overhead of task creation
    if (n < 20) return fib_serial(n);

    #pragma omp task shared(i) firstprivate(n)
    i = fib_task(n - 1);

    #pragma omp task shared(j) firstprivate(n)
    j = fib_task(n - 2);

    #pragma omp taskwait
    return i + j;
}

int main() {
    int n = 30;
    int result_task, result_serial;

    double tstart = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            result_task = fib_task(n);
        }
    }
    double tstop = omp_get_wtime();
    double time_task = (tstop - tstart) * 1000.0;

    tstart = omp_get_wtime();
    result_serial = fib_serial(n);
    tstop = omp_get_wtime();
    double time_serial = (tstop - tstart) * 1000.0;

    printf("=== Exercise 05: Fibonacci Task Parallelization ===\n");
    printf("Fibonacci(%d) Task Result   : %d (Time: %.3f ms)\n", n, result_task, time_task);
    printf("Fibonacci(%d) Serial Result : %d (Time: %.3f ms)\n", n, result_serial, time_serial);
    printf("Speedup                       : %.2fx\n", time_serial / time_task);

    return 0;
}
