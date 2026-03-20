#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0)
            printf("Użycie: %s liczba_punktow\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    long long N = atoll(argv[1]);
    long long local_N = N / size;
    long long local_inside = 0;

    srand(time(NULL) + rank);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    for (long long i = 0; i < local_N; i++) {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            local_inside++;
        }
    }

    long long global_inside = 0;

    MPI_Reduce(&local_inside, &global_inside, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        double pi = 4.0 * global_inside / N;
        printf("Przybliżenie pi = %f\n", pi);
        printf("Czas = %f s\n", end - start);
    }

    MPI_Finalize();
    return 0;
}