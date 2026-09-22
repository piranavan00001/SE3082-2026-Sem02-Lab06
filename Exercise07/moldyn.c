#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NP 108
#define NSTEPS 10

typedef struct {
    double x, y, z;
    double vx, vy, vz;
    double fx, fy, fz;
} Particle;

Particle p[NP];
double epot, vir;

void init_particles() {
    for (int i = 0; i < NP; i++) {
        p[i].x = (i % 6) * 1.2;
        p[i].y = ((i / 6) % 6) * 1.2;
        p[i].z = (i / 36) * 1.2;
        p[i].vx = 0.1 * (i % 3 - 1);
        p[i].vy = 0.1 * (i % 5 - 2);
        p[i].vz = 0.1 * (i % 7 - 3);
        p[i].fx = 0.0; p[i].fy = 0.0; p[i].fz = 0.0;
    }
}

void compute_forces() {
    epot = 0.0;
    vir = 0.0;

    for (int i = 0; i < NP; i++) {
        p[i].fx = 0.0; p[i].fy = 0.0; p[i].fz = 0.0;
    }

    #pragma omp parallel for reduction(+:epot, vir) schedule(static)
    for (int i = 0; i < NP - 1; i++) {
        for (int j = i + 1; j < NP; j++) {
            double rx = p[i].x - p[j].x;
            double ry = p[i].y - p[j].y;
            double rz = p[i].z - p[j].z;
            double r2 = rx * rx + ry * ry + rz * rz + 1e-4;

            if (r2 < 9.0) {
                double r2i = 1.0 / r2;
                double r6i = r2i * r2i * r2i;
                double fij = 48.0 * r6i * (r6i - 0.5) * r2i;

                #pragma omp critical
                {
                    p[i].fx += rx * fij; p[i].fy += ry * fij; p[i].fz += rz * fij;
                    p[j].fx -= rx * fij; p[j].fy -= ry * fij; p[j].fz -= rz * fij;
                }

                epot += 4.0 * r6i * (r6i - 1.0);
                vir -= fij * r2;
            }
        }
    }
}

int main() {
    printf("=== Exercise 07: Molecular Dynamics (Argon Atoms) OpenMP Benchmark ===\n");
    printf("%-10s %-15s %-15s %-15s\n", "Threads", "Potential Eng", "Virial", "Time (ms)");

    for (int threads = 1; threads <= 4; threads++) {
        omp_set_num_threads(threads);
        init_particles();

        double tstart = omp_get_wtime();
        for (int step = 0; step < NSTEPS; step++) {
            compute_forces();
        }
        double tstop = omp_get_wtime();
        double time_taken = (tstop - tstart) * 1000.0;

        printf("%-10d %-15.4f %-15.4f %-15.2f\n", threads, epot, vir, time_taken);
    }

    return 0;
}
