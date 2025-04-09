#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina masina;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** tabela;
};
typedef struct HashTable HashTable;

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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
	while (cap) {
		afisareMasina(cap->masina);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod* cap, Masina masinaNoua) {
	Nod* p = cap;
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	p->next = nou;
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0;i < dimensiune;i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* nume, int dimensiune) {
	int suma = 0;
	for (int i = 0;i < strlen(nume);i++) {
		suma += nume[i];
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.tabela[pozitie] == NULL) {
		hash.tabela[pozitie] = (Nod*)malloc(sizeof(Nod));
		hash.tabela[pozitie]->masina = masina;
		hash.tabela[pozitie]->next = NULL;
	}
	else {
		adaugaMasinaInLista(hash.tabela[pozitie], masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	HashTable hash = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0;i < ht.dim;i++) {
		if (ht.tabela[i] != NULL) {
			printf("\nMasinile de pe pozitia %d sunt:\n", i);
			afisareListaMasini(ht.tabela[i]);
		}
		else {
			printf("\nPe pozitia %d nu avem masini\n");
		}
	}
}

void dezalocareListaMasini(Nod** cap) {
	Nod* p = *cap;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model != NULL) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer != NULL) {
			free(aux->masina.numeSofer);
		}
		free(aux);
	}
	*cap = NULL;
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i = 0;i < ht->dim;i++) {
		dezalocareListaMasini(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

float calculeazaMedieLista(Nod* cap) {
	float suma = 0;
	int nrElemente = 0;
	while (cap) {
		suma += cap->masina.pret;
		nrElemente++;
		cap = cap->next;
	}
	if (nrElemente > 0) {
		return suma / nrElemente;
	}
	else {
		return 0;
	}
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturi = NULL;
	*nrClustere = 0;
	for (int i = 0;i < ht.dim;i++) {
		if (ht.tabela[i] != NULL) {
			(*nrClustere)++;
		}
	}
	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0;i < ht.dim;i++) {
		if (ht.tabela[i] != NULL) {
			preturi[contor] = calculeazaMedieLista(ht.tabela[i]);
			contor++;
		 }
	}
	return preturi;
}

Masina getMasinaDinLista(Nod* cap, const char* nume) {
	Masina m;
	m.id = - 1;
	while (cap && strcmp(cap->masina.numeSofer, nume) != 0) {
		cap = cap->next;
	}
	if (cap) {
		m = cap->masina;
		m.model = (char*)malloc(strlen(cap->masina.model) + 1);
		strcpy_s(m.model, strlen(cap->masina.model) + 1, cap->masina.model);
		m.numeSofer = (char*)malloc(strlen(cap->masina.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(cap->masina.numeSofer) + 1, cap->masina.numeSofer);

	}
	return m;
}

Masina getMasinaDupaNumeSofer(HashTable ht, const char* numeCautat) {
	Masina m;
	m.id = -1;
	int poz = calculeazaHash(numeCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim) {
		return getMasinaDinLista(ht.tabela[poz], numeCautat);
	}
	return m;
}

void stergereMasinaDupaNumeSofer(HashTable* ht, const char* numeCautat) {
	int poz = calculeazaHash(numeCautat, ht->dim);
	Nod* p = ht->tabela[poz];
	Nod* anterior = NULL;

	while (p && strcmp(p->masina.numeSofer, numeCautat) != 0) {
		anterior = p;
		p = p->next;
	}
	if (p) {
		if (anterior == NULL) {
			ht->tabela[poz] = p->next;
		}
		else {
			anterior->next = p->next;
		}
		free(p->masina.model);
		free(p->masina.numeSofer);
		free(p);
	}
}

int modificaPretMasina(HashTable ht, const char* nume, float pretNou) {
	int poz = calculeazaHash(nume, ht.dim);
	Nod* p = ht.tabela[poz];
	while (p) {
		if (strcmp(p->masina.numeSofer, nume) == 0) {
			p->masina.pret = pretNou;
			return 1;
		}
		p = p->next;
	}
	return 0;
}

int numarTotalMasini(HashTable ht) {
	int total = 0;
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.tabela[i];
		while (p) {
			total++;
			p = p->next;
		}
	}
	return total;
}


void afisareMasiniCuMaiMultDeXUsi(HashTable ht, int prag) {
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.tabela[i];
		while (p) {
			if (p->masina.nrUsi > prag) {
				afisareMasina(p->masina);
			}
			p = p->next;
		}
	}
}


int main() {

	HashTable ht = citireMasiniDinFisier("masini.txt", 7);
	afisareTabelaDeMasini(ht);
	int nrClusere = 0;
	float *preturi = calculeazaPreturiMediiPerClustere(ht, &nrClusere);

	printf("\nPreturile medii per clustere sunt:\n");
	for (int i = 0;i < nrClusere;i++) {
		printf("%.2f ", preturi[i]);
	}
	printf("\n");

	Masina masina = getMasinaDupaNumeSofer(ht, "Gigel Gigel");
	if (masina.id != -1) {
		printf("Masina Cautata:\n");
		afisareMasina(masina);
		if (masina.model != NULL) {
			free(masina.model);
		}
		if (masina.numeSofer != NULL) {
			free(masina.numeSofer);
		}
	}
	else {
		printf("Masina nu a fost gasita. Soferul nu are nicio masina.\n");
	}

	const char* numeModificat = "Gheorghe Gigel";
	printf("\n=== Modificare pret masina (nume: %s) ===\n", numeModificat);
	if (modificaPretMasina(ht, numeModificat, 9999.99f)) {
		printf("Pret modificat cu succes pentru %s\n", numeModificat);
	}
	else {
		printf("Nu s-a gasit masina pentru %s\n", numeModificat);
	}
	afisareTabelaDeMasini(ht);

	printf("\n=== Masini cu mai mult de 4 usi ===\n");
	afisareMasiniCuMaiMultDeXUsi(ht, 4);

	int totalMasini = numarTotalMasini(ht);
	printf("\n=== Numar total masini: %d ===\n", totalMasini);

	dezalocareTabelaDeMasini(&ht);
	return 0;
}