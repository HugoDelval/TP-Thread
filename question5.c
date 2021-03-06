#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mtxCpt;
static FILE* f;


static void print_prime_factors(uint64_t n)
{
	printf("%ju : ", n);
	
    uint64_t i;
	if(n!=1)
	{
		uint64_t fin = n;
		for( i=2 ; i<=fin && n!=1 ; i++)
		{
			while(n%i == 0)
			{
				printf("%ju ", i);
				n/=i;
			}
				
		}
		printf("\n");
	}
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
	f= fopen("question3.court.txt", "r");
	
	pthread_create(&thread1, NULL, gestion_threads, NULL );
	pthread_create(&thread2, NULL, gestion_threads, NULL );
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	
    fclose(f);

    return 0;
}
