#include<mpi.h>
#include<string.h>
#include<stdio.h>

int main(void){
	int A[5][5]={0};
	int B[5][5]={0};
	int myrank=0;
	int size=0;
	int i, j;
	MPI_Datatype columntype;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	

	if(myrank==0){
		for(i=0;i<5; i++){
			for(j=0;j<5; j++){
				A[i][j]=i*5+j;
				printf("%d ",A[i][j] );
			}
			printf("\n" );
		}
		printf("---------------------\n");
	}
	if(myrank==1){
		for(i=0;i<5; i++){
			for(j=0;j<5; j++){
				B[i][j]=i*5+j+50;
				printf("%d ",B[i][j] );
			}
			printf("\n" );
		}
		printf("--------------------\n");
	}

	MPI_Type_vector(5,1,5,MPI_INT,&columntype);
	MPI_Type_commit(&columntype);

	if(myrank==0){
		MPI_Bcast(&A[0][3], 1, columntype, 0, MPI_COMM_WORLD);
		MPI_Bcast(&A[0][4], 1, columntype, 1, MPI_COMM_WORLD);
	}else{
		MPI_Bcast(&B[0][4], 1, columntype, 0, MPI_COMM_WORLD);
		MPI_Bcast(&B[0][2], 1, columntype, 1, MPI_COMM_WORLD);
	}
	if(myrank==1){
		printf("최종결과\n");
		for(i=0;i<5; i++){
			for(j=0;j<5; j++){
			printf("%d ",B[i][j] );
			}
			printf("\n" );
		}
	}
	MPI_Finalize();
}