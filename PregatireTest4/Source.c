#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* next;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f))
		{
			adaugaMasinaInLista(&cap, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	while (*cap) {
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.model) {
			free(p->info.model);
		}
		if (p->info.numeSofer) {
			free(p->info.numeSofer);
		}
		free(p);
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;

	while (cap) {
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor > 0) {
		return suma / contor;
	}
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if ((*cap)) {
		Nod* p = *cap;
		while (p) {
			while (p->next && p->next->info.serie != serieCautata) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.numeSofer) {
					free(aux->info.numeSofer);
				}
				if (aux->info.model) {
					free(aux->info.model);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}

}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

Nod* inversareLista(Nod* cap) {
	Nod* anterior = NULL;
	Nod* curent = cap;
	Nod* urmator = NULL;
	while (curent) {
		urmator = curent->next;
		curent->next = anterior;
		anterior = curent;
		curent = urmator;
	}
	return anterior;
}

void afiseazaMasiniDupaModel(Nod* cap, const char* modelCautat) {
	while (cap) {
		if (strcmp(cap->info.model, modelCautat) == 0) {
			afisareMasina(cap->info);
		}
		cap = cap->next;
	}
}

int numarMasiniCuNrUsi(Nod* cap, int nrUsi) {
	int count = 0;
	while (cap) {
		if (cap->info.nrUsi == nrUsi)
			count++;
		cap = cap->next;
	}
	return count;
}

int main() {

	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("\n\nPretul mediu este: %.2f\n", calculeazaPretMediu(cap));
	printf("\n\nPretul masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Gigel"));

	cap = inversareLista(cap);
	printf("\nLista inversata:\n");
	afisareListaMasini(cap);

	printf("\nMasini de model:\n");
	afiseazaMasiniDupaModel(cap, "Astra");

	printf("\nNumar masini cu 4 usi: %d\n", numarMasiniCuNrUsi(cap, 4));

	printf("\nStergere seria A:\n");
	stergeMasiniDinSeria(&cap, 'A');
	afisareListaMasini(cap);
	printf("\nStergere seria B:\n");
	stergeMasiniDinSeria(&cap, 'B');
	afisareListaMasini(cap);


	dezalocareListaMasini(&cap);


	return 0;
}