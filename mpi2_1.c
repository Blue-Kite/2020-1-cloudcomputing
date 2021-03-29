#include<mpi.h>
#include<string.h>
#include<stdio.h>

void main(void){
	int A[10][10];
	int myrank=0;
	int size=0;
	int i, j;
	
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(myrank==0){
		for(i=0;i<10; i++){
			for(j=0;j<10; j++){
				A[i][j]=i*10+j;
			}
		}
	}
	MPI_Bcast(A,100,MPI_INT, 0, MPI_COMM_WORLD);
	
	if(myrank==1){
		for(i=0;i<10; i++){
			for(j=0;j<10; j++){
				printf("%d",A[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
}