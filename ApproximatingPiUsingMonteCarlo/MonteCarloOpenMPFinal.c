/* OpenMP version to calculate value of PI using Mandel-Brot Method
*/

#include <omp.h>
#include <stdio.h>

#define RNG_MOD 0x80000000
int state;

int rng_int(void);
double rng_doub(double range);

int main(int argc,char** argv) {
    int i, numIn,N, n;
    double x, y, pi;
    N=atoi(argv[1]);
    n = atol(argv[2]);
    numIn = 0;
    double start=omp_get_wtime();
    #pragma omp threadprivate(state)
    #pragma omp num_threads(N) parallel private(x, y) reduction(+:numIn) 
    {

        state = 25234 + 17 * omp_get_thread_num();
        #pragma omp for
        for (i = 0; i <= n; i++) {
            x = (double)rng_doub(1.0);
            y = (double)rng_doub(1.0);
            if (x*x + y*y <= 1) numIn++;
        }
    }
    pi = 4.*numIn / n;
    printf(" %lf\n",omp_get_wtime()-start);
    return 0;
}

int rng_int(void) {
   // & 0x7fffffff is equivalent to modulo with RNG_MOD = 2^31
   return (state = (state * 1103515245 + 12345) & 0x7fffffff);
}

double rng_doub(double range) {
    return ((double)rng_int()) / (((double)RNG_MOD)/range);
}
