#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Użycie: %s liczba_punktow\n", argv[0]);
        return 1;
    }

    long long N = atoll(argv[1]);
    long long inside = 0;

    srand(time(NULL));

    for (long long i = 0; i < N; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }

    double pi = 4.0 * inside / N;
    printf("Przybliżenie pi = %f\n", pi);

    return 0;
}