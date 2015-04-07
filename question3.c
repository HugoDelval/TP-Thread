#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

void * print_prime_factors(void * np)
{
	uint64_t* pn =(uint64_t*) (np);
	uint64_t n = (*pn);
    uint64_t i;
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
	
	return 0;
}

int main(void)
{
    FILE* f = fopen("question3.court.txt", "r");
    char ligne [50];
    uint64_t tab_nombre [10000];
    int taille=0;
    while( fgets(ligne,sizeof(ligne),f) )
    {
		tab_nombre[taille] = ( (uint64_t)atoll(ligne) );
		taille++;
	}
	
	int i;
	if(taille%2 == 1)
	{
		taille--;
		print_prime_factors(&tab_nombre[taille]);
	}
	
	pthread_t thread1; 
	pthread_t thread2; 
	
	for(i=0 ; i< taille -1 ; i+=2)
	{
		pthread_create(&thread1, NULL, print_prime_factors, &tab_nombre[i] );
		pthread_create(&thread2, NULL, print_prime_factors, &tab_nombre[i+1] );
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		
	}
	

    return 0;
}
