#include<mpi.h>
#include<string.h>
#include<stdio.h>

int main(void){
	double R[102][102]={0};
     
	int size=0;
	int i, j,k;
	int local_size=0; //한프로세스가 맡을 가로길이
	int local_n=1;    //한프로세스가 맡을 것을 첫번째 인덱스 
	MPI_Datatype columntype; //데이터타입선언
	MPI_Status status;
	int myrank=0;
	int n1,n2,m1,m2;
	int tag1=5;
	int tag2=6;

	MPI_Init(NULL,NULL); 
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);//랭크
	MPI_Comm_size(MPI_COMM_WORLD,&size); //총프로세스갯수
	
	local_size=100/size;  //100를 총프로세스갯수로나눔
	local_n=1+local_size*myrank;	
	
	int source=myrank-1;
	int dest=myrank+1;

	for(j=40; j<60; j++){  //벽난로
		R[0][j]=200.0;
	}
	
	MPI_Type_vector(100,1,102,MPI_DOUBLE, &columntype); //블록갯수100개 데이터갯수1개 간격 102 더블타입을 묶어서 columntype으로 생각할 것
	MPI_Type_commit(&columntype);

	if(myrank==0){ //프로세스0번은 사용자가 입력을 받음 
		printf("출력할구간");
		scanf("%d %d %d %d",&m1,&m2,&n1,&n2);
	}

	if(myrank==0){
		for(k=0; k<10000; k++){ //만번돌림
			for(i=1; i<101; i++){
				for(j=1; j<=local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]); //맡은 분량만큼계산 
				}
			}
		MPI_Send(&R[1][local_n+local_size-1], 1, columntype, 1, 0, MPI_COMM_WORLD); //맡은 부분의 맨끝을 1번 프로세스에게 보냄 
		MPI_Recv(&R[1][local_n+local_size], 1, columntype, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //1번프로세스에게서 받은걸 자기분량 바로 옆 세로줄에 담음 
		}
		
	}else if(myrank==size-1){ //마지막 프로세스 
		for(k=0; k<10000; k++){
			MPI_Send(&R[1][local_n], 1, columntype, myrank-1, 0, MPI_COMM_WORLD); //자기가 맡은 분량의 첫번째 세로줄을 전프로세스에게 보냄 
			MPI_Recv(&R[1][local_n-1], 1, columntype, myrank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //전프로세스에게서 자기가 맡은것 직전 세로줄을 받음 
			for(i=1; i<101; i++){
				for(j=local_n; j<local_n+local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);		
				}
			}
		}
	}else{//처음과 끝이 아닌 프로세스
		for(k=0; k<10000; k++){
			MPI_Send(&R[1][local_n], 1, columntype, source, 0, MPI_COMM_WORLD); //자신이 맡은부분의 첫번째줄을 전 프로세스에게 보냄 
			MPI_Recv(&R[1][local_n-1], 1, columntype, source,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //전프로세스에게서 직전것을 받음 
			MPI_Recv(&R[1][local_n+local_size], 1, columntype, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //자신이 맡은것의 끝 직전 다음것을 다음 프로세스에게서 받음 
			for(i=1; i<101; i++){
				for(j=local_n; j<local_n+local_size; j++){
					R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				
				}
			}
			MPI_Send(&R[1][local_n+local_size-1], 1, columntype, dest, 0, MPI_COMM_WORLD); //자신이 맡은 것의 끝분량을 다음프로세스에게 받음 
		}
	}

	if(myrank==0){
		for(i=1; i<=size-1; i++){ //0번프로세스는 자신빼고 모든 프로세스에게서 
			for(j=0; j<=local_size-1; j++){  //각프로세스의 첫번째줄에서 마지막줄에 해당하는 부분을 해당하는 인덱스에 받아옴 
				MPI_Recv(&R[1][local_n+local_size*i+j],1 , columntype, i ,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}else{
		for(j=local_n; j<=local_n+local_size-1; j++){ //모든프로세스들이 자기가 맡아서 계산한것을 0번에게 보냄 local_n부터 local_n+local_size-1 
			MPI_Send(&R[1][j], 1 , columntype, 0, 0, MPI_COMM_WORLD);
		}
	}

	if(myrank==0){ //입력받은 부분을 출력함 
		for(i=m1; i<=m2; i++){
			for(j=n1; j<=n2; j++){
				printf("%7.1f",R[i][j]);
				printf("\n");
			}
		}
	}
	
	MPI_Finalize(); //mpi끝
}