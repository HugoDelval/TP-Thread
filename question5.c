#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mtxCpt;
static FILE* f;

void * print_prime_factors(void * np) //np=null
{
    uint64_t i;
    
    char ligne [50];
    
    pthread_mutex_lock(&mtxCpt);
    while(fgets(ligne,sizeof(ligne),f) !=NULL)
    {
		pthread_mutex_unlock(&mtxCpt);
		
		uint64_t n = (uint64_t)atoll(ligne);
		printf("%ju : ", n);
		
		if(n!=1 && n!=2)
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
	f= fopen("question3.long.txt", "r");
	
	pthread_create(&thread1, NULL, print_prime_factors, NULL );
	pthread_create(&thread2, NULL, print_prime_factors, NULL );
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	
    fclose(f);

    return 0;
}
