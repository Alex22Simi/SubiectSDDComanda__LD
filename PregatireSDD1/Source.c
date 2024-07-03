#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct Comanda
{
	unsigned int id;//%hu
	//nr intreg pozitiv - unsigned
	char* status;
	char* nume;
	unsigned char numarProd;//%hhu
	float suma;
}Comanda;

typedef struct Nod
{
	Comanda* info;
	struct Nod* next;
	struct Nod* prev;
}Nod;

Comanda* initializareComanda(unsigned int id, char* status, char* nume, unsigned char numarProd, float suma)
{
	Comanda* comanda = (Comanda*)malloc(sizeof(Comanda));
	comanda->id = id;
	comanda->status = (char*)malloc(strlen(status) + 1);
	strcpy(comanda->status, status);
	comanda->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(comanda->nume, nume);
	comanda->numarProd = numarProd;
	comanda->suma = suma;
	return comanda;
}

Nod* initializareNod(Comanda* info)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = info;
	nod->next = NULL;
	nod->prev = NULL;
	return nod;
}

Nod* inserareSfarsitLD(Nod* cap, Nod** coada, Comanda* comanda)
{
	Nod* nodNou = initializareNod(comanda);
	if (cap == NULL)
	{
		cap = nodNou;
		*coada = nodNou;
	}
	else
	{
		(*coada)->next = nodNou;
		nodNou->prev = *coada;
		*coada = nodNou;
		
	}
	return cap;
}
Nod* inserareInceputLD(Nod* cap, Nod** coada, Comanda* comanda)
{
	Nod* nodNou = initializareNod(comanda);
	if (cap != NULL)
	{
		cap = nodNou;
		*coada = nodNou;
	}
	else
	{
		nodNou->next = cap;
		cap->prev = nodNou;
		cap = nodNou;
	}
	return cap;
}
//inserare alternanta - criteriu ales de noi
Nod* inserareAlternantaLD(Nod* cap, Nod** coada, Comanda* comanda, int* alternanta)
{
	Nod* nodNou = initializareNod(comanda);
	if (cap == NULL)
	{
		cap = nodNou;
		*coada = nodNou;
	}
	else if(*alternanta % 2 == 0)
	{
		nodNou->next = cap;
		cap->prev = nodNou;
		cap = nodNou;
	}
	else
	{
		(*coada)->next = nodNou;
		nodNou->prev = *coada;
		*coada = nodNou;
	}
	(*alternanta)++;
	return cap;
}


void afisareLD(Nod* cap)
{
	if (cap != NULL)
	{
		while (cap != NULL)
		{
			printf("%hu %s %s %hhu %f\n", cap->info->id, cap->info->status,
				cap->info->nume, cap->info->numarProd, cap->info->suma);
			cap = cap->next;
		}
	}
}


void afisareInversaLD(Nod* coada)
{
	if (coada != NULL)
	{
		while (coada != NULL)
		{
			printf("%hu %s %s %hhu %f\n", coada->info->id, coada->info->status,
				coada->info->nume, coada->info->numarProd, coada->info->suma);
			coada = coada->prev;
		}
	}
}
void dezalocareLD(Nod* cap)
{
	if (cap != NULL)
	{
		while (cap != NULL)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info->status);
			free(temp->info->nume);
			free(temp->info);
			free(temp);
		}
		printf("Dezalocare cu succes.");
	}
}

float valoareMedie(Nod* cap, char* status)
{
	float valoare = 0;
	int nrComenzi = 0;
	while (cap != NULL)
	{
		if (strcmp(cap->info->status, status) == 0)
		{
			valoare += cap->info->numarProd * cap->info->suma;
			nrComenzi++;
		}
		cap = cap->next;
	}
	return valoare / nrComenzi;
}

//0- daca are duplicate, 1- daca nu are
int existaDuplicate(Nod* cap)
{
	Nod* temp = cap;
	while (temp != NULL)
	{
		Nod* temp2 = temp->next;
		while (temp2 != NULL)
		{
			if (temp->info->id == temp2->info->id)
			{
				return 1;//avem dublicat
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
	return 0;//nu avem dublicat
}

int nrClientiUnici(Nod* cap)
{
	int duplicate = 0;
	int totalClienti = 0;
	Nod* temp = cap;
	while (temp != NULL)
	{
		Nod* temp2 = temp->next;
		while (temp2 != NULL)
		{
			if (strcmp(temp->info->nume, temp2->info->nume ) == 0)
			{
				duplicate++;
				break;
			}
			
			temp2 = temp2->next;

		}
		totalClienti++;
		temp = temp->next;
	}
	return totalClienti - duplicate;
	
}






void main()
{
	
	unsigned int id;//%hu
	//nr intreg pozitiv - unsigned
	char status[20];
	char nume[20];
	unsigned char numarProd;//%hhu
	float suma;
	Nod* cap = NULL;
	Nod* coada = NULL;
	FILE* f = fopen("Comanda.txt", "r");
	int alternanta = 0;
	while (fscanf(f, "%hu %s %s %hhu %f", &id, &status, &nume, &numarProd, &suma) == 5)
	{
		Comanda* comanda = initializareComanda(id, status, nume, numarProd, suma);
		cap = inserareAlternantaLD(cap, &coada, comanda, &alternanta);

	}
	fclose(f);
	afisareLD(cap);
	printf("------------------------\n");
	afisareInversaLD(coada);
	printf("-----------------------\n");

	float valoare = 0;
	valoare = valoareMedie(cap, "livrata");
	printf("%f\n", valoare);

	int areDublicate = 0;
	areDublicate = existaDuplicate(cap);
	printf("%d\n", areDublicate);
	
	int clientiUnici = 0;
	clientiUnici = nrClientiUnici(cap);
	printf("%d\n", clientiUnici);
	







	dezalocareLD(cap);
	
}