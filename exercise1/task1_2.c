#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define ITER 100000
#define MSG_SIZE 8

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char msg[MSG_SIZE] = "1234567";
    char rec[MSG_SIZE];

    MPI_Request request;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for(int i = 0; i < ITER; i++) {

        if(rank == 0) {

            MPI_Irecv(rec, MSG_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &request);

            MPI_Rsend(msg, MSG_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }

        if(rank == 1) {

            MPI_Irecv(rec, MSG_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request);

            MPI_Rsend(msg, MSG_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if(rank == 0) {

        double time = end - start;

        double total_bytes = (double)ITER * 2 * MSG_SIZE;
        double bandwidth_MB = total_bytes / (1024.0 * 1024.0) / time;

        printf("Total time: %f s\n", time);
        printf("Bandwidth: %f MB/s\n", bandwidth_MB);
    }

    MPI_Finalize();
}