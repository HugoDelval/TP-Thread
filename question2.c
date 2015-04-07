#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
	
	return NULL;
}

int main(void)
{
    FILE* f = fopen("question3.court.txt", "r");
    char ligne [50];
    while( fgets(ligne,sizeof(ligne),f) )
    {
		long long a = atoll(ligne);
		print_prime_factors( &(a) );
	}
	
	

    return 0;
}
