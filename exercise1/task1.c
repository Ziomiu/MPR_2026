#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int len;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);

    int msg = 12345;

    if (rank == 0) {
//send then recieve
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Rsend(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Send 0 %d \n",msg);
        MPI_Request request;
        int rec;
        MPI_Irecv(&rec, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Wait(&request,MPI_STATUS_IGNORE);
        printf("Received 0  %d \n",rec);
    }

    if (rank == 1) {
//recieve then send
        int rec;
        MPI_Request request;
        MPI_Irecv(&rec, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Wait(&request,MPI_STATUS_IGNORE);
        printf("Received 1 %d \n",rec);

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Rsend(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Send 1 %d \n",msg);

    }

    MPI_Finalize();
}