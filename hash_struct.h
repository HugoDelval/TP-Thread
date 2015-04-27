#ifndef HASH_STRUCT
#define HASH_STRUCT

#define MAX_FACTORS 63

#define NB_MAX_NOMBRES 2000


static pthread_mutex_t mtxTabPartage;

typedef struct
{
	uint64_t nombre_decompose;
	int indice_dernier_facteur;
	uint64_t facteurs_premiers[MAX_FACTORS];
} decomposition;


typedef struct{
	decomposition decompo_avec_meme_hash[NB_MAX_NOMBRES];
	int indice_dernier;
} case_hashes;


static case_hashes toutes_les_decompos[NB_MAX_NOMBRES];


static int getHash(uint64_t n)
{
	return n%NB_MAX_NOMBRES;
}

static int checkTableHashage(uint64_t nombre_a_decomposer, uint64_t* factors, int offset_factors)
{
	int j;
	int hash = getHash(nombre_a_decomposer);
	case_hashes candidats = toutes_les_decompos[hash];
	for(j=0 ; j<=candidats.indice_dernier ; j++)
	{
		decomposition decompo_candidate = candidats.decompo_avec_meme_hash[j];
		if(decompo_candidate.nombre_decompose == nombre_a_decomposer)
		{
			// on a trouve le nombre
			int k;
			for(k=0 ; k<= decompo_candidate.indice_dernier_facteur ; k++)
			{
				factors[k+offset_factors+1] = decompo_candidate.facteurs_premiers[k];
			}
			return decompo_candidate.indice_dernier_facteur + offset_factors;
		}
	}
	return offset_factors;
}

int insererNombre(uint64_t nombre_a_decomposer, uint64_t* factors)
{
	int nb_factors=0;
	if(n!=1)
	{
		/* on verifie qu'on a pas deja decompose le nombre */
		nb_factors = checkTableHashage(nombre_a_decomposer, factors, nb_factors);
		/* fin verification */
		
		/*  */
		uint64_t i;
		uint64_t fin = nombre_a_decomposer;
		for( i=2 ; i<=fin && n!=1 ; i++)
		{
			if(n%i == 0)
			{
				/* ajout tableau partage */
				pthread_mutex_lock(&mtxTabPartage);
				tab_partage[nb_factors_tab]=i;
				nb_factors_tab++;
				pthread_mutex_unlock(&mtxTabPartage);
				/* */
				while(n%i == 0)
				{
					factors[nb_factors]=i;
					nb_factors++;
					n/=i;
				}
			}
			
		}
	}
	return nb_factors;
}

#endif
