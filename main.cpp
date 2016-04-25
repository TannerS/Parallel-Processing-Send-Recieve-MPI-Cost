

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>


#define SIZE 100000000

// argc = cpu count, argv = file.cpp
int main(int argc, char *argv[])
{
	srand(time(0));
	double t0, t1;
	int n = 0;
	int* list;
	int msgtag;
	float cost = 0;

	int myid, numprocs;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;
	//initialize MPI execution environment
	MPI_Init(&argc, &argv);
	//each process get total # of processes 
	//the total # of processes specified in mpirun –np n
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	//each process gets its own id
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	// Gets the name of the processor
	MPI_Get_processor_name(processor_name, &namelen);
	// display info
	fprintf(stderr, "process %d on %s\n", myid, processor_name);
	fflush(stderr);

	list = new int[SIZE];

	if (myid == 0)
	{
		for (int i = 0; i < SIZE; i++)
		{
			list[i] = rand() % 100 + 1;
		}


		t0 = MPI_Wtime();
		// blocking send to reciever
		/*
		int MPI_Send(const void *buf, int count, MPI_Datatype 	datatype, int dest, int tag, MPI_Comm comm)
		buf:	initial address of send buffer
		count: number of elements in send buffer
		datatype: datatype of each send buffer element
		dest: rank of destination
		tag: message tag  (keeps track of who is sending it, different cpus can send infp, need a way to sort it)
		comm: communicator
		*/

		//thi weil lsend data to the dest recieve call
		MPI_Send(list, SIZE, MPI_INT, 1, 12345, MPI_COMM_WORLD);
		// this will wait here until a recieve
		MPI_Recv(list, SIZE, MPI_INT, 1, 67890, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		t1 = MPI_Wtime();
		cost = (t1 - t0) / 2.0;

		fprintf(stderr, "cost %f\n", cost);
		fflush(stderr);

	}
	else // slave, do as I command and do thee!
	{
		/*
		int MPI_Recv(void *buf, int count, 	MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
		Here: buf and status are output parameters
		buf: (initial address of) data received
		status: status object
		status object contains three fields named 		MPI_SOURCE, MPI_TAG, and MPI_ERROR)
		*/

		//this will wait here until it recieves a call, then execute 
		MPI_Recv(list, SIZE, MPI_INT, 0, 12345, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		MPI_Send(list, SIZE, MPI_INT, 0, 67890, MPI_COMM_WORLD);
	}

	delete[] list;

	MPI_Finalize();

}
