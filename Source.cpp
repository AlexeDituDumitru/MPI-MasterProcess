// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include <time.h> 



#define noOfProcceses 5

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request[2];
	srand(time(NULL));
	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//printf("Start with %d /n", rank);
	bool isMaster = false;
	bool isOut = false;
	int other;
	int generatedNumber = rand() % 100;
	while (isMaster ==  false) {
		for (int i = 0; i < noOfProcceses ; i++) {
			if (i == noOfProcceses - 1) {
				MPI_Isend(&generatedNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[1]);
			}
			else {
				MPI_Isend(&generatedNumber, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD, &request[1]);
			}
			int receivedNumber;
			if (i == 0) {
				MPI_Irecv(&receivedNumber, 1, MPI_INT, noOfProcceses - 1, 0, MPI_COMM_WORLD, &request[0]);
				other = noOfProcceses - 1;
			}
			else {
				MPI_Irecv(&receivedNumber, 1, MPI_INT, i - 1, 0, MPI_COMM_WORLD, &request[0]);
				other = i - 1;
			}
			if (receivedNumber > generatedNumber) {
				isOut = true;
				isMaster = false;
				generatedNumber = receivedNumber;
			}
			else {
					if (isOut == false && receivedNumber == generatedNumber) {
						isMaster = true;
						printf("Process %d is master with %d \n", rank, generatedNumber);
				}
			}
		}
	}
	MPI_Waitall(2, request, &status);


	MPI_Finalize();
	return 0;
}

