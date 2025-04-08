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

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

//creare structura pentru Lista Dubla 
struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};

typedef struct ListaDubla ListaDubla;

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

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = lista->first;
	nou->prev = NULL;
	if (lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer) {
			free(aux->masina.numeSofer);
		}
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.nrNoduri>0) {
		float suma = 0;
		Nod* p = lista.first;
		while (p) {
			suma += p->masina.pret;
			p = p->next;
		}
		return suma/lista.nrNoduri;
	}
	return 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL) {
		return;
	}
	Nod* p = lista->first;
	while (p!=NULL && p->masina.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}
	if (p->prev == NULL) {
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else {
		p->prev->next = p->next;
	}
	if (p->next != NULL) {
		p->next->prev = p->prev;
	}
	else {
		lista->last = p->prev;
	}
	if (p->masina.model) {
		free(p->masina.model);
	}
	if (p->masina.numeSofer) {
		free(p->masina.numeSofer);
	}
	free(p);
	lista->nrNoduri--;

}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->masina.pret > max->masina.pret) {
				max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(max->masina.numeSofer) + 1);
		strcpy(nume, max->masina.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

void inserareSortataDupaPret(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->prev = NULL;
	nou->next = NULL;

	if (lista->first == NULL) {
		lista->first = lista->last = nou;
	}
	else if (masinaNoua.pret < lista->first->masina.pret) {
		nou->next = lista->first;
		lista->first->prev = nou;
		lista->first = nou;
	}
	else {
		Nod* p = lista->first;
		while (p->next && p->next->masina.pret < masinaNoua.pret) {
			p = p->next;
		}
		nou->next = p->next;
		nou->prev = p;
		if (p->next) {
			p->next->prev = nou;
		}
		else {
			lista->last = nou;
		}
		p->next = nou;
	}

	lista->nrNoduri++;
}

void cautaMasiniDupaModel(ListaDubla lista, const char* model) {
	Nod* p = lista.first;
	while (p) {
		if (strcmp(p->masina.model, model) == 0) {
			afisareMasina(p->masina);
		}
		p = p->next;
	}
}

void stergeMasiniDupaModel(ListaDubla* lista, const char* model) {
	Nod* p = lista->first;
	while (p) {
		if (strcmp(p->masina.model, model) == 0) {
			Nod* deSters = p;
			if (p->prev) {
				p->prev->next = p->next;
			}
			else {
				lista->first = p->next;
			}
			if (p->next) {
				p->next->prev = p->prev;
			}
			else {
				lista->last = p->prev;
			}
			p = p->next;
			free(deSters->masina.model);
			free(deSters->masina.numeSofer);
			free(deSters);
			lista->nrNoduri--;
		}
		else {
			p = p->next;
		}
	}
}

int main() {

	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	stergeMasinaDupaID(&lista, 10);
	printf("\n\nDe la sfarsit:\n");
	afisareListaMasiniDeLaSfarsit(lista);
	printf("\n\nPretul mediu al masinilor este %.2f:\n", calculeazaPretMediu(lista));
	
	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("\n\nSoferul cu cea mai scumpa masina este: %s\n", numeSofer);
	if (numeSofer != NULL) {
		free(numeSofer);
	}

	Masina masinaNoua;
	masinaNoua.id = 100;
	masinaNoua.nrUsi = 4;
	masinaNoua.pret = 21500.0f;
	masinaNoua.model = malloc(strlen("Logan") + 1);
	strcpy(masinaNoua.model, "Logan");
	masinaNoua.numeSofer = malloc(strlen("Ion") + 1);
	strcpy(masinaNoua.numeSofer, "Ion");
	masinaNoua.serie = 'L';

	inserareSortataDupaPret(&lista, masinaNoua);
	printf("\n\nLista dupa inserare sortata dupa pret:\n");
	afisareListaMasiniDeLaInceput(lista);

	// ----------------------- CAUTARE -----------------------
	printf("\n\nMasini cu modelul 'Logan':\n");
	cautaMasiniDupaModel(lista, "Logan");

	// ----------------------- STERGERE DUPA MODEL -----------------------
	printf("\n\nStergem toate masinile cu modelul 'Logan'\n");
	stergeMasiniDupaModel(&lista, "Logan");
	printf("\n\nLista dupa stergere:\n");
	afisareListaMasiniDeLaInceput(lista);

	dezalocareLDMasini(&lista);



	return 0;
}