#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int len;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &len);

    int msg = 123;
    int recv_msg;

    if(size < 2){
        if(rank==0) printf("Need at least 2 processes\n");
        MPI_Finalize();
        return 0;
    }

    if(rank == 0){

        printf("Process 0 on %s sending\n", hostname);

        MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 0 received back: %d\n", recv_msg);
    }

    if(rank == 1){

        MPI_Recv(&recv_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 on %s received %d\n", hostname, recv_msg);

        recv_msg += 1;

        MPI_Send(&recv_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}