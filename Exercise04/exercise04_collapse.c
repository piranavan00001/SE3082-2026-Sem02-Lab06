#include <omp.h>
#include <stdio.h>

int main() {
    printf("=== Collapsed #pragma omp parallel for collapse(2) ===\n");
    #pragma omp parallel for collapse(2)
    for (int r = 1; r <= 5; r++) {
        for (int k = 1; k <= 10; k++) {
            printf("r = %d, k = %d, thread # = %d\n", r, k, omp_get_thread_num());
        }
    }
    return 0;
}
