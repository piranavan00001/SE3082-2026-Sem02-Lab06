#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        int th = omp_get_thread_num();
        printf("thread # %d\n", th);

        #pragma omp barrier

        printf("Outside Barrier # %d\n", omp_get_thread_num());
        printf("Hi again from thread # %d\n", omp_get_thread_num());
    }
    return 0;
}
