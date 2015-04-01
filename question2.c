#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void print_prime_factors(uint64_t n)
{
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
			if(i%100000000 == 0)
				printf(" etape: %ju ", i);
				
		}
		printf("\n");
	}
}

int main(void)
{
    FILE* f = fopen("small.txt", "r");
    char ligne [50];
    while( fgets(ligne,sizeof(ligne),f) )
    {
		print_prime_factors( (uint64_t)atoll(ligne) );
	}
	
	

    return 0;
}
