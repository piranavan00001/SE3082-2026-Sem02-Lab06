#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        int th = omp_get_thread_num();
        printf("thread # %d\n", th);

        #pragma omp master
        {
            printf("Inside Master # %d\n", omp_get_thread_num());
            printf("Exiting Master \n");
        }
        printf("Hi again from thread # %d\n", omp_get_thread_num());
    }
    return 0;
}
