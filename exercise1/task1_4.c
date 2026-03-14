#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ITER 100000
#define MAX_SIZE (8*1024*1024)

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char *msg = malloc(MAX_SIZE);
    char *rec = malloc(MAX_SIZE);

    MPI_Request request;

    for(int size = 1; size <= MAX_SIZE; size *= 2) {

        MPI_Barrier(MPI_COMM_WORLD);

        double start = MPI_Wtime();

        for(int i = 0; i < ITER; i++) {

            if(rank == 0) {

                MPI_Irecv(rec, size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &request);

                MPI_Rsend(msg, size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

                MPI_Wait(&request, MPI_STATUS_IGNORE);
            }

            if(rank == 1) {

                MPI_Irecv(rec, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request);

                MPI_Rsend(msg, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

                MPI_Wait(&request, MPI_STATUS_IGNORE);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        double end = MPI_Wtime();

        if(rank == 0) {

            double time = end - start;

            double total_bytes = (double)ITER * 2 * size;

            double bandwidth_MB =
                    total_bytes / (1024.0 * 1024.0) / time;

            printf("%d %f\n", size, bandwidth_MB);
        }
    }

    free(msg);
    free(rec);

    MPI_Finalize();
}