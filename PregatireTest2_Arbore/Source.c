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
	Nod* st;
	Nod* dr;
	Masina info;
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

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {

	if ((*radacina) != NULL) {
		if (masinaNoua.id < (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		}
		else if (masinaNoua.id > (*radacina)->info.id)
		{
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		}
		else {
			if ((*radacina)->info.model != NULL) {
				free((*radacina)->info.model);
			}
			if ((*radacina)->info.numeSofer != NULL) {
				free((*radacina)->info.numeSofer);
			}
			(*radacina)->info.nrUsi = masinaNoua.nrUsi;
			(*radacina)->info.pret = masinaNoua.pret;
			(*radacina)->info.serie = masinaNoua.serie;
			(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
			strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
			(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
			strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		}
	}
	else {
		(*radacina) = malloc(sizeof(Nod));
		(*radacina)->info = masinaNoua;
		(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
		strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
		(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArbore(&radacina, m);
		free(m.model);
		free(m.numeSofer);
	}
	fclose(file);
	return radacina;
}


void afisareArborePreOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisareArborePreOrdine(radacina->st);
		afisareArborePreOrdine(radacina->dr);
	}
}


void afisareMasiniInOrdine(Nod* radacina) {
	if (radacina != NULL)
	{
		afisareMasiniInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareMasiniInOrdine(radacina->dr);
	}
}

void afisareArborePostOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArborePostOrdine(radacina->st);
		afisareArborePostOrdine(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("\n--- Afisare in ordine ---\n");
	afisareMasiniInOrdine(radacina);

	printf("\n--- Afisare preordine ---\n");
	afisareArborePreOrdine(radacina);

	printf("\n--- Afisare postordine ---\n");
	afisareArborePostOrdine(radacina);
}


void dezalocareArboreDeMasini(Nod** radacina) {
	if ((*radacina) != NULL)
	{
		dezalocareArboreDeMasini(&((*radacina)->st));
		dezalocareArboreDeMasini(&((*radacina)->dr));
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	m.id = -1;
	if (id > radacina->info.id)
	{
		return getMasinaByID(radacina->dr, id);
	}
	else if (id < radacina->info.id) {
		return getMasinaByID(radacina->st, id);
	}
	else {
		m = radacina->info;
		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		m.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);
		return m;
	}

	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina != NULL) {
		return  1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	else {
		return 0;
	}
}


int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL) return 0;
	int st = calculeazaInaltimeArbore(radacina->st);
	int dr = calculeazaInaltimeArbore(radacina->dr);
	return 1 + (st > dr ? st : dr);
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL) return 0;
	return radacina->info.pret +
		calculeazaPretTotal(radacina->st) +
		calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL) return 0;
	float suma = 0;
	if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
		suma += radacina->info.pret;
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
	return suma;
}

void cautaMasiniDupaModel(Nod* radacina, const char* model) {
	if (radacina == NULL) return;
	if (strcmp(radacina->info.model, model) == 0)
		afisareMasina(radacina->info);
	cautaMasiniDupaModel(radacina->st, model);
	cautaMasiniDupaModel(radacina->dr, model);
}

int modificaPretDupaID(Nod* radacina, int id, float pretNou) {
	if (radacina == NULL) return 0;
	if (id < radacina->info.id)
		return modificaPretDupaID(radacina->st, id, pretNou);
	else if (id > radacina->info.id)
		return modificaPretDupaID(radacina->dr, id, pretNou);
	else {
		radacina->info.pret = pretNou;
		return 1;
	}
}

int numaraMasiniCuNrUsi(Nod* radacina, int nrUsi) {
	if (radacina == NULL) return 0;
	int count = 0;
	if (radacina->info.nrUsi == nrUsi) count = 1;
	return count +
		numaraMasiniCuNrUsi(radacina->st, nrUsi) +
		numaraMasiniCuNrUsi(radacina->dr, nrUsi);
}

void afiseazaMasiniCuPretMaiMare(Nod* radacina, float pretMinim) {
	if (radacina == NULL) return;
	afiseazaMasiniCuPretMaiMare(radacina->st, pretMinim);
	if (radacina->info.pret > pretMinim)
		afisareMasina(radacina->info);
	afiseazaMasiniCuPretMaiMare(radacina->dr, pretMinim);
}

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	afisareMasiniDinArbore(arbore);

	int idCautat = 2;
	Masina m = getMasinaByID(arbore, idCautat);
	printf("\n--- Masina cu ID-ul %d ---\n", idCautat);
	if (m.id != -1)
		afisareMasina(m);
	free(m.model);
	free(m.numeSofer);

	printf("\nNumar total de noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));
	printf("Pret total al masinilor: %.2f\n", calculeazaPretTotal(arbore));

	const char* sofer = "Ionescu";
	printf("Pret total masini pentru soferul '%s': %.2f\n", sofer, calculeazaPretulMasinilorUnuiSofer(arbore, sofer));

	int idDeModificat = 3;
	float pretNou = 8000.0f;
	if (modificaPretDupaID(arbore, idDeModificat, pretNou)) {
		printf("\nPretul masinii cu ID %d a fost modificat la %.2f\n", idDeModificat, pretNou);
	}
	else {
		printf("\nNu s-a gasit nicio masina cu ID %d pentru modificare.\n", idDeModificat);
	}

	int nrUsi = 4;
	printf("Numar masini cu %d usi: %d\n", nrUsi, numaraMasiniCuNrUsi(arbore, nrUsi));

	const char* modelCautat = "BMW";
	printf("\n--- Masini cu modelul '%s' ---\n", modelCautat);
	cautaMasiniDupaModel(arbore, modelCautat);

	float pragPret = 10000.0f;
	printf("\n--- Masini cu pret mai mare decat %.2f ---\n", pragPret);
	afiseazaMasiniCuPretMaiMare(arbore, pragPret);

	dezalocareArboreDeMasini(&arbore);

	return 0;
}