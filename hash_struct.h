#ifndef HASH_STRUCT
#define HASH_STRUCT

#define MAX_FACTORS 63

#define NB_MAX_NOMBRES 2000

/* j'ai l'impression que
ca ne sert a rien.. quand on rajoute un nombre dans la table,
on increment la taille en dernier, du coup pas de probleme */
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

int insererNombre(uint64_t nombre_a_decomposer, uint64_t* factors)
{
	int nb_factors=0;
	if(nombre_a_decomposer!=1)
	{
		/* on verifie qu'on a pas deja decompose le nombre */
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
					factors[k] = decompo_candidate.facteurs_premiers[k];
				}
				return decompo_candidate.indice_dernier_facteur+1;
			}
		}
		/* fin verification */
		
		/* ce nombre n'est pas notre table ! il va falloir l'ajouter */
		decomposition nouvelle_decompo;
		nouvelle_decompo.nombre_decompose = nombre_a_decomposer;
		nouvelle_decompo.indice_dernier_facteur=-1;

		/*  */
		uint64_t i;
		uint64_t fin = nombre_a_decomposer;
		for( i=2 ; i<=fin && nombre_a_decomposer!=1 ; i++)
		{
			while(nombre_a_decomposer%i == 0)
			{
				/* ajout a la nouvelle decomposition partagee */
				nouvelle_decompo.indice_dernier_facteur ++;
				nouvelle_decompo.facteurs_premiers[nouvelle_decompo.indice_dernier_facteur] = i;
				/* fin ajout */

				factors[nb_factors]=i;
				nb_factors++;
				nombre_a_decomposer/=i;

				/* on verifie que le nouveau nombre n'est pas dans la table */
				int j;
				int hash = getHash(nombre_a_decomposer);
				case_hashes candidats = toutes_les_decompos[hash];
				for(j=0 ; j<=candidats.indice_dernier ; j++)
				{
					decomposition decompo_candidate = candidats.decompo_avec_meme_hash[j];
					if(decompo_candidate.nombre_decompose == nombre_a_decomposer)
					{
						// on a trouve le nombre :
						//    il faut ajouter sa decomposition a factors
						//   ET a la decomposition actuelle ! 
						int k;
						for(k=0 ; k<= decompo_candidate.indice_dernier_facteur ; k++)
						{
							uint64_t nouveau_facteur = decompo_candidate.facteurs_premiers[k]; 
							factors[k + nb_factors] = nouveau_facteur;
							
							nouvelle_decompo.indice_dernier_facteur ++;
							nouvelle_decompo.facteurs_premiers[nouvelle_decompo.indice_dernier_facteur] = nouveau_facteur;
						}
						nb_factors += decompo_candidate.indice_dernier_facteur+1;
						nombre_a_decomposer = 1; // pour sortir du while && du 'for' sur les entiers positifs
						break; // pour sortir du 'for' sur candidats
					}
				}
				/* fin verif */

			}
			
		}

		pthread_mutex_lock(&mtxTabPartage);
		toutes_les_decompos[hash].indice_dernier ++;
		toutes_les_decompos[hash].decompo_avec_meme_hash[toutes_les_decompos[hash].indice_dernier] = nouvelle_decompo;
		pthread_mutex_unlock(&mtxTabPartage);

	}
	return nb_factors;
}

void initialiserTableHashage()
{
	int i;
	for(i=0 ; i<NB_MAX_NOMBRES ; ++i)
	{
		toutes_les_decompos[i].indice_dernier = -1;
	}
}

#endif
