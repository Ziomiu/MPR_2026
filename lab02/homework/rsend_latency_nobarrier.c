#include <stdio.h>
#include <mpi.h>

#define ITER 100000

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char msg;
    char rec;

    MPI_Request request;

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    for (int i = 0; i < ITER; i++) {

        if (rank == 0) {

            MPI_Irecv(&rec, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &request);
            MPI_Rsend(&msg, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }

        if (rank == 1) {

            MPI_Irecv(&rec, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request);
            MPI_Rsend(&msg, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {

        double end = MPI_Wtime();
        double time = (end - start) / (2.0 * ITER);

        FILE *f = fopen("rsend_latency_nobarriers.txt", "w");
        fprintf(f, "%f\n", time * 1000);
        fclose(f);
    }

    MPI_Finalize();
}