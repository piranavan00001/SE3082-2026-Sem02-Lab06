#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NPOINTS 1000
#define MAXITER 2000

struct d_complex {
    double r;
    double i;
};

int main() {
    int numoutside = 0;
    double area, error;
    double eps = 1.0e-5;

    printf("=== Exercise 07: Area of Mandelbrot Set Benchmark ===\n");
    printf("%-10s %-15s %-15s %-15s\n", "Threads", "Area", "Outside Pts", "Time (ms)");

    for (int threads = 1; threads <= 4; threads++) {
        omp_set_num_threads(threads);
        numoutside = 0;

        double tstart = omp_get_wtime();

        #pragma omp parallel for reduction(+:numoutside) private(eps) schedule(dynamic)
        for (int i = 0; i < NPOINTS; i++) {
            for (int j = 0; j < NPOINTS; j++) {
                struct d_complex c;
                c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS);
                c.i = 1.25 * (double)(j) / (double)(NPOINTS);
                
                struct d_complex z = c;
                int iter;
                for (iter = 0; iter < MAXITER; iter++) {
                    double temp = z.r * z.r - z.i * z.i + c.r;
                    z.i = 2.0 * z.r * z.i + c.i;
                    z.r = temp;
                    if ((z.r * z.r + z.i * z.i) > 4.0) {
                        numoutside++;
                        break;
                    }
                }
            }
        }

        double tstop = omp_get_wtime();
        double time_taken = (tstop - tstart) * 1000.0;
        
        area = 2.0 * 2.5 * 1.25 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
        printf("%-10d %-15.6f %-15d %-15.2f\n", threads, area, numoutside, time_taken);
    }

    return 0;
}
