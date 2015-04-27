#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "hash_struct.h"

static pthread_mutex_t mtxCpt;
static FILE* f;
static pthread_mutex_t mtxAffichage;


static void print_prime_factors(uint64_t n)
{
	
	uint64_t factors[MAX_FACTORS];
	int j,k;
	k=insererNombre(n,factors);
	
	pthread_mutex_lock(&mtxAffichage);
	printf("%ju: ",n);
	for(j=0; j<k; j++)
	{
		printf("%ju ",factors[j]);
	}
	pthread_mutex_unlock(&mtxAffichage);
	
	printf("\n");
}


static void * gestion_threads(void * np) //np=null
{
    
    char ligne [50];
    
    pthread_mutex_lock(&mtxCpt);
    while(fgets(ligne,sizeof(ligne),f) !=NULL)
    {
		pthread_mutex_unlock(&mtxCpt);
		uint64_t n = (uint64_t)atoll(ligne);
		print_prime_factors(n);
		pthread_mutex_lock(&mtxCpt);
	}
	
	pthread_mutex_unlock(&mtxCpt);
	
	return 0;
}

int main(void)
{
	pthread_t thread1;
	pthread_t thread2; 
	pthread_t thread3;
	pthread_t thread4; 
	
	pthread_mutex_init(&mtxCpt, NULL);
	f= fopen("small.txt", "r");
	
	pthread_create(&thread1, NULL, gestion_threads, NULL );
	pthread_create(&thread2, NULL, gestion_threads, NULL );
	pthread_create(&thread3, NULL, gestion_threads, NULL );
	pthread_create(&thread4, NULL, gestion_threads, NULL );
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	
	
    fclose(f);

    return 0;
}
