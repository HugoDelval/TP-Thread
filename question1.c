#include <stdio.h>
#include <stdint.h>

void print_prime_factors(uint64_t n)
{
	uint64_t i;
    printf("%ju : ", n);
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
			if(i%100000000 == 0)
				printf(" etape: %ju ", i);
				
		}
		printf("\n");
	}
}

int main(void)
{
    print_prime_factors(77); // expected result:   77: 7 11
    print_prime_factors(84); // expected result:   84: 2 2 3 7

    // expected result:   429496729675917: 3 18229 7853726291
    print_prime_factors(429496729675917);


    return 0;
}
