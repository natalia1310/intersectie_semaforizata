#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define RED "\e[1;31m"
#define GRN "\e[1;32m"
#define YEL "\e[1;33m"
#define ENDER "\033[0m"

struct semafor_vehicule{
	bool verde;
	bool galben;
	bool rosu;
};
typedef struct semafor_vehicule SEM_V;

struct semafor_pietoni{
	bool verde;
	bool rosu;
};
typedef struct semafor_pietoni SEM_P;

SEM_P P[4];
SEM_V V[4];
sem_t SemPerecheA, SemTranzitieAB, SemPerecheB, SemTranzitieBA;

void* PerecheA(void *);
void* TranzitieAB(void *);
void* PerecheB(void *);
void* TranzitieBA(void *);

void Intersectie(); // Ilustrare grafica a intersectiei (in culori)

void* (*functie[])(void*) = {PerecheA, TranzitieAB, PerecheB, TranzitieBA};

int main(void)
{
	pthread_t FIR[4];
	int i;
	
	sem_init(&SemPerecheA, 1, 0);
	sem_init(&SemTranzitieAB, 1, 0);
	sem_init(&SemPerecheB, 1, 0);
	sem_init(&SemTranzitieBA, 1, 0);
	
	for(i=0; i<4;i++)
		pthread_create(FIR+i,NULL,(void*)(*(functie+i)),NULL);
		
	sem_post(&SemPerecheA);
	
	for(i=0; i<4; i++)
		pthread_join(*(FIR+i), NULL);
		
	printf("Main: Toate firele de executie s-au terminat! \n");
	pthread_exit(NULL);
}

void* PerecheA(void *arg)
{
	int k;
	while(1)
	{
		sem_wait(&SemPerecheA);
		for(k = 0; k < 2; k++)
		{
			V[k].verde = true;
			V[k].rosu = false;
			V[k].galben = false;
			
			P[k].verde = false;
			P[k].rosu = true;
		}
		for(k = 2; k < 4; k++)
		{
			V[k].verde = false;
			V[k].rosu = true;
			V[k].galben = false;
			
			P[k].verde = true;
			P[k].rosu = false;
		}
		Intersectie();
		sleep(10);
		sem_post(&SemTranzitieAB);
	}
}

void* TranzitieAB(void *arg)
{
	int k;
	while(1)
	{
		sem_wait(&SemTranzitieAB);
		for(k = 0; k < 2; k++)
		{
			V[k].verde = false;
			V[k].rosu = false;
			V[k].galben = true;
			
			P[k].verde = false;
			P[k].rosu = true;
		}
		for(k = 2; k < 4; k++)
		{
			V[k].verde = false;
			V[k].rosu = true;
			V[k].galben = false;
			
			P[k].verde = false;
			P[k].rosu = true;
		}
		Intersectie();
		sleep(5);
		sem_post(&SemPerecheB);
	}
}

void* PerecheB(void *arg)
{
	int k;
	while(1)
	{
		sem_wait(&SemPerecheB);
		for(k = 0; k < 2; k++)
		{
			V[k].verde = false;
			V[k].rosu = true;
			V[k].galben = false;
			
			P[k].verde = true;
			P[k].rosu = false;
		}
		for(k = 2; k < 4; k++)
		{
			V[k].verde = true;
			V[k].rosu = false;
			V[k].galben = false;
			
			P[k].verde = false;
			P[k].rosu = true;
		}
		Intersectie();
		sleep(10);
		sem_post(&SemTranzitieBA);
	}
}

void* TranzitieBA(void *arg)
{
	int k;
	while(1)
	{
		sem_wait(&SemTranzitieBA);
		for(k = 0; k < 2; k++)
		{
			V[k].verde = false;
			V[k].rosu = true;
			V[k].galben = false;
			
			P[k].verde = false;
			P[k].rosu = true;
		}
		for(k = 2; k < 4; k++)
		{
			V[k].verde = false;
			V[k].rosu = false;
			V[k].galben = true;
			
			P[k].verde = false;
			P[k].rosu = true;
		}
		Intersectie();
		sleep(5);
		sem_post(&SemPerecheA);
	}
}

void Intersectie()
{
	system("clear");
    printf("	                       |    \\/     |    /\\     |\n");
    printf("	                       |    \\/     |    /\\     |\n");
    printf("	                       |    \\/    ");
 
    if(P[0].rosu) printf(RED);
	else if(P[0].verde) printf(GRN);
    printf("[P0]");
    printf(ENDER);
 
    printf("  /\\     |\n");
    printf("	                       |    \\/     |    /\\     |\n");
    printf("---------------------------------- ");
 
    if(V[0].rosu) printf(RED);
	else if(V[0].verde) printf(GRN);
	else if(V[0].galben) printf(YEL);
    printf("[V0]");
    printf(ENDER);
 
    printf(" -------------------------------------- \n");
    printf("                               |                       |\n");
    printf("             <<<<<<<<          |                      ");
 
    if(V[2].rosu) printf(RED);
	else if(V[2].verde) printf(GRN);
	else if(V[2].galben) printf(YEL);
    printf("[V2]");
    printf(ENDER);
 
    printf("       <<<<<<<<\n");
    printf("                               |                       |\n");
    printf("--------------------");
 
    if(P[3].rosu) printf(RED);
	else if(P[3].verde) printf(GRN);
    printf("[P3]");
    printf(ENDER);
 
    printf("--------------------------------------");
 
    if(P[2].rosu) printf(RED);
	else if(P[2].verde) printf(GRN);
    printf("[P2]");
    printf(ENDER);
 
    printf("------------ \n");
    printf("                               |                       |\n");
    printf("             >>>>>>>>         ");
 
    if(V[3].rosu) printf(RED);
	else if(V[3].verde) printf(GRN);
	else if(V[3].galben) printf(YEL);
    printf("[V3]");
    printf(ENDER);
 
    printf("                     |         >>>>>>>>\n");
    printf("                               |                       |\n");
    printf("---------------------------------------------- ");
 
    if(V[1].rosu) printf(RED);
	else if(V[1].verde) printf(GRN);
	else if(V[1].galben) printf(YEL);
    printf("[V1]");
    printf(ENDER);
 
    printf(" -------------------------- \n");
    printf("	                       |    \\/     |    /\\     |\n");
    printf("	                       |    \\/    ");
 
    if(P[1].rosu) printf(RED);
	else if(P[1].verde) printf(GRN);
    printf("[P1]");
    printf(ENDER);
 
    printf("  /\\     |\n");
    printf("	                       |    \\/     |    /\\     |\n");
    printf("	                       |    \\/     |    /\\     |\n");
}