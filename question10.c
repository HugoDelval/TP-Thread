#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mtxCpt;
static FILE* f;
#define MAX_FACTORS 63
#define NB_MAX_NOMBRES 2000
static uint64_t tab_partage[MAX_FACTORS*NB_MAX_NOMBRES]; 
static pthread_mutex_t mtxTabPartage;
static pthread_mutex_t mtxAffichage;
static int nb_factors_tab = 0;

static int get_prime_factors(uint64_t n,uint64_t* factors)
{
	uint64_t i;
	int nb_factors=0;
	if(n!=1)
	{
		for(i=0 ; i<nb_factors_tab ; i++)
		{
			pthread_mutex_lock(&mtxTabPartage);
			uint64_t nb_candidat = tab_partage[i];
			pthread_mutex_unlock(&mtxTabPartage);
			while(n%nb_candidat == 0)
			{
				factors[nb_factors]=nb_candidat;
				nb_factors++;
				n/=nb_candidat;
			}
		}
		
		uint64_t fin = n;
		int passe = 1;
		for( i=2 ; i<=fin && n!=1 ; i++)
		{
			while(n%i == 0)
			{
				if(passe == 1)
				{
					/* ajout tableau partage */
					pthread_mutex_lock(&mtxTabPartage);
					tab_partage[nb_factors_tab]=i;
					nb_factors_tab++;
					pthread_mutex_unlock(&mtxTabPartage);
					/* */
					passe = 0;
				}
				factors[nb_factors]=i;
				nb_factors++;
				n/=i;
			}			
		}
	}
	return nb_factors;
}


static void print_prime_factors(uint64_t n)
{
	
	uint64_t factors[MAX_FACTORS];
	int j,k;
	k=get_prime_factors(n,factors);
	
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
	
	pthread_mutex_init(&mtxCpt, NULL);
	f= fopen("small.txt", "r");
	
	pthread_create(&thread1, NULL, gestion_threads, NULL );
	pthread_create(&thread2, NULL, gestion_threads, NULL );
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	
	
    fclose(f);

    return 0;
}
