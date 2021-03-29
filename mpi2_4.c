#include<mpi.h>
#include<string.h>
#include<stdio.h>

int main(void){
	double R[102][102]={0};
     
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
	
	local_size=100/size;
	local_n=1+local_size*myrank;	
	
	int source=myrank-1;
	int dest=myrank+1;

	for(j=40; j<60; j++){
		R[0][j]=200.0;
	}
	
	MPI_Type_vector(100,1,102,MPI_DOUBLE, &columntype);
	MPI_Type_commit(&columntype);

	if(myrank==0){
		printf("출력할구간");
		scanf("%d %d %d %d",&m1,&m2,&n1,&n2);
	}

	if(myrank==0){
		for(k=0; k<10000; k++){
			for(i=1; i<101; i++){
				for(j=1; j<=local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				}
			}
		MPI_Send(&R[1][local_n+local_size-1], 1, columntype, 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&R[1][local_n+local_size], 1, columntype, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		
	}else if(myrank==size-1){
		for(k=0; k<10000; k++){
			MPI_Send(&R[1][local_n], 1, columntype, myrank-1, 0, MPI_COMM_WORLD);
			MPI_Recv(&R[1][local_n-1], 1, columntype, myrank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(i=1; i<101; i++){
				for(j=local_n; j<local_n+local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);		
				}
			}
		}
	}else{
		for(k=0; k<10000; k++){
			MPI_Send(&R[1][local_n], 1, columntype, source, 0, MPI_COMM_WORLD);
			MPI_Recv(&R[1][local_n-1], 1, columntype, source,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&R[1][local_n+local_size], 1, columntype, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(i=1; i<101; i++){
				for(j=local_n; j<local_n+local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				
				}
			}
			MPI_Send(&R[1][local_n+local_size-1], 1, columntype, dest, 0, MPI_COMM_WORLD);
		}
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
		for(i=m1; i<=m2; i++){
			for(j=n1; j<=n2; j++){
				printf("%7.1f",R[i][j]);
				printf("\n");
			}
		}
	}
	
	MPI_Finalize();
}