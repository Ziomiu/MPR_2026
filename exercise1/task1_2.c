#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <mpi.h>


int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank;
    int iters = 100000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    uint64_t msg = 12345678;
    uint64_t rec;

    MPI_Request request;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < iters; i++) {

        if (rank == 0) {

            MPI_Irecv(&rec, 1, MPI_UINT64_T, 1, 0, MPI_COMM_WORLD, &request);

            MPI_Rsend(&msg, 1, MPI_UINT64_T, 1, 0, MPI_COMM_WORLD);

            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }

        if (rank == 1) {

            MPI_Irecv(&rec, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, &request);

            MPI_Rsend(&msg, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);

            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {

        double time = end - start;

        double total_bytes = (double) iters * 2 * 8;
        double bandwidth_MB = total_bytes / (1024.0 * 1024.0) / time;

        printf("Total time: %f s\n", time);
        printf("Bandwidth: %f MB/s\n", bandwidth_MB);
    }

    MPI_Finalize();
}