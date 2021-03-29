#include<mpi.h>
#include<string.h>
#include<stdio.h>

void main(void){
	int myrank, size, result;
	int number=0;
	int i=0;
	int sendcount=0;
	int A[1000];
	int irecv[1000];
	
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	sendcount=1000/size;

	if(myrank==0){
		for(i=0;i<1000;i++){
			A[i]=i+1;
		}
	}
	MPI_Scatter(A,sendcount,MPI_INT, irecv,sendcount,MPI_INT, 0, MPI_COMM_WORLD);
	for(i=0;i<sendcount;i++){
		if(irecv[i]%2!=0){
			number=number+1;
		}
	}
	MPI_Reduce(&number,&result,1,MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);
	if(myrank==0){
		printf("number:%d", result);
	}
	MPI_Finalize();
}
