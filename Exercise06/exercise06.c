#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define STRIP_SIZE 256  // Strip size aligned with cache line / SIMD vector width

int main() {
    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));
    double *C_serial = (double *)malloc(N * sizeof(double));
    double *C_parallel = (double *)malloc(N * sizeof(double));

    // Initialize arrays
    for (int i = 0; i < N; i++) {
        A[i] = i * 1.5;
        B[i] = i * 2.5;
    }

    // Serial Computation
    double tstart = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        C_serial[i] = A[i] * B[i];
    }
    double tstop = omp_get_wtime();
    double time_serial = (tstop - tstart) * 1000.0;

    // Parallel Computation using Strip Mining
    tstart = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i += STRIP_SIZE) {
        int limit = (i + STRIP_SIZE < N) ? (i + STRIP_SIZE) : N;
        for (int j = i; j < limit; j++) {
            C_parallel[j] = A[j] * B[j];
        }
    }
    tstop = omp_get_wtime();
    double time_parallel = (tstop - tstart) * 1000.0;

    // Verify Results
    int correct = 1;
    for (int i = 0; i < N; i++) {
        if (C_serial[i] != C_parallel[i]) {
            correct = 0;
            break;
        }
    }

    printf("=== Exercise 06: Element-wise Array Multiplication (Strip Mining) ===\n");
    printf("Array Size (N)             : %d elements\n", N);
    printf("Strip Size                 : %d elements\n", STRIP_SIZE);
    printf("Serial Execution Time      : %.4f ms\n", time_serial);
    printf("Parallel Execution Time    : %.4f ms\n", time_parallel);
    printf("Speedup                    : %.2fx\n", time_serial / time_parallel);
    printf("Verification               : %s\n", correct ? "SUCCESS (Results match)" : "FAILED");

    free(A); free(B); free(C_serial); free(C_parallel);
    return 0;
}
