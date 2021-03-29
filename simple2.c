#include<mpi.h>
#include<string.h>
#include<stdio.h>

int main(void){
	double R[12][12]={0};
     
	int size=0;
	int i, j,k;
	int local_size=0;
	int local_n=1;
	MPI_Datatype columntype;
	MPI_Status status;
	int myrank=0;
	int n1,n2,m1,m2;
	int tag1=5;
	int tag2=6;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	local_size=10/size;
	local_n=1+local_size*myrank;	
	
	int source=myrank-1;
	int dest=myrank+1;

	for(j=0; j<12; j++){
		R[0][j]=200.0;
	}
	
	MPI_Type_vector(10,1,12,MPI_DOUBLE, &columntype);
	MPI_Type_commit(&columntype);


	if(myrank==0){
		
			for(i=1; i<11; i++){
				for(j=1; j<=local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				}
			}
			MPI_Send(&R[1][local_n+local_size-1], 1, columntype, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&R[1][local_n+local_size], 1, columntype, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		
	}else if(myrank==size-1){
			MPI_Send(&R[1][local_n], 1, columntype, myrank-1, 0, MPI_COMM_WORLD);
			MPI_Recv(&R[1][local_n-1], 1, columntype, myrank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(i=1; i<11; i++){
				for(j=local_n; j<local_n+local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				}
			}
		
	}else{

			MPI_Send(&R[1][local_n], 1, columntype, source, 0, MPI_COMM_WORLD);
			MPI_Recv(&R[1][local_n-1], 1, columntype, source,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&R[1][local_n+local_size], 1, columntype, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(i=1; i<11; i++){
				for(j=local_n; j<local_n+local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				}
			}
			MPI_Send(&R[1][local_n+local_size-1], 1, columntype, dest, 0, MPI_COMM_WORLD);
		
	}

	if(myrank==0){
		for(i=1; i<=size-1; i++){
			for(j=0; j<=local_size-1; j++){
				MPI_Recv(&R[1][local_n+local_size*i+j],1 , columntype, i ,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}else{
		for(j=local_n; j<=local_n+local_size-1; j++){
			MPI_Send(&R[1][j], 1 , columntype, 0, 0, MPI_COMM_WORLD);
		}
	}

	if(myrank==0){
		printf("myrank: %d\n", myrank);
		for(i=0; i<12; i++){
			for(j=0; j<12; j++){
				printf("%.1f  ", R[i][j]);
				
			}
			printf("\n");
		}
		printf("\n");
	}
	if(myrank==1){
		printf("myrank: %d\n", myrank);
		for(i=0; i<12; i++){
			for(j=0; j<12; j++){
				printf("%.1f  ", R[i][j]);
				
			}
			printf("\n");
		}
		printf("\n");
	}
	
	MPI_Finalize();
}