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
	Nod* st;
	Nod* dr;
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

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
	}
	else {
		return 0;
	}
}
void rotireStanga(Nod** radacina) {
	Nod* nod;
	nod = (*radacina)->dr;
	(*radacina)->dr = nod->st;
	nod->st = *radacina;
	*radacina = nod;
}
void rotireDreapta(Nod** radacina) {
	Nod* aux;
	aux = (*radacina)->st;
	(*radacina)->st = aux->dr;
	aux->dr = (*radacina);
	*radacina = aux;
}

int gradEchilibru(Nod* radacina) {
	if (radacina != NULL) {
		return(calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr));
	}
	else {
		return 0;
	}
}
void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->st), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
		}
		int grad = gradEchilibru((*radacina));
		if (grad == 2)
		{
			if (gradEchilibru((*radacina)->st) == 1) {
				rotireDreapta(radacina);
			}
			else {
				rotireStanga(&((*radacina)->st));
				rotireDreapta(radacina);
			}
		}
		else if (grad == -2) {
			if (gradEchilibru((*radacina)->dr) == 1) {
				rotireDreapta(&((*radacina)->dr));

			}
			rotireStanga(radacina);
		}
	}
	else {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = masinaNoua;
		nod->dr = NULL;
		nod->st = NULL;
		(*radacina) = nod;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArboreEchilibrat(&radacina, m);
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

void afisareArborePostOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArborePostOrdine(radacina->st);
		afisareArborePostOrdine(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void afisareArboreInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArboreInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareArboreInOrdine(radacina->dr);
	}
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



float calculeazaPretTotal(Nod* radacina) {
	if (radacina != NULL) {
		float pretSubarboreStang = calculeazaPretTotal(radacina->st);
		float pretSubarboreDrept = calculeazaPretTotal(radacina->dr);
		float pretTotal = pretSubarboreStang + pretSubarboreDrept + radacina->info.pret;
		return pretTotal;
	}
	else {
		return 0;
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina != NULL) {
		float pretSt = calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
		float pretDr = calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			return radacina->info.pret + pretSt + pretDr;
		}
		return pretSt + pretDr;
	}
	return 0;
}

int numaraMasiniCuNrUsi(Nod* radacina, int nrUsi) {
	if (radacina == NULL)
		return 0;

	int countSt = numaraMasiniCuNrUsi(radacina->st, nrUsi);
	int countDr = numaraMasiniCuNrUsi(radacina->dr, nrUsi);

	if (radacina->info.nrUsi == nrUsi)
		return 1 + countSt + countDr;
	else
		return countSt + countDr;
}

void afiseazaMasiniCuPretMaiMare(Nod* radacina, float prag) {
	if (radacina) {
		afiseazaMasiniCuPretMaiMare(radacina->st, prag);
		if (radacina->info.pret > prag) {
			afisareMasina(radacina->info);
		}
		afiseazaMasiniCuPretMaiMare(radacina->dr, prag);
	}
}

void cautaMasiniDupaModel(Nod* radacina, const char* model) {
	if (radacina) {
		cautaMasiniDupaModel(radacina->st, model);
		if (strcmp(radacina->info.model, model) == 0) {
			afisareMasina(radacina->info);
		}
		cautaMasiniDupaModel(radacina->dr, model);
	}
}

int modificaPretDupaID(Nod* radacina, int id, float pretNou) {
	if (radacina == NULL)
		return 0;

	if (id < radacina->info.id)
		return modificaPretDupaID(radacina->st, id, pretNou);
	else if (id > radacina->info.id)
		return modificaPretDupaID(radacina->dr, id, pretNou);
	else {
		radacina->info.pret = pretNou;
		return 1;
	}
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	printf("Arborele in preordine:\n");
	afisareArborePreOrdine(arbore);

	printf("Afisare in INORDINE (id crescator):\n");
	afisareArboreInOrdine(arbore);

	printf("\nAfisare in POSTORDINE:\n");
	afisareArborePostOrdine(arbore);

	printf("Numar masini cu 4 usi: %d\n", numaraMasiniCuNrUsi(arbore, 4));

	printf("Masinile cu pret mai mare de 20000:\n");
	afiseazaMasiniCuPretMaiMare(arbore, 20000.0f);

	if (modificaPretDupaID(arbore, 3, 25000.0f)) {
		printf("Pretul a fost modificat cu succes pentru masina cu ID 3.\n");
	}
	else {
		printf("Nu s-a gasit masina cu ID 3.\n");
	}

	printf("Pretul total al masinilor: %.2f\n", calculeazaPretTotal(arbore));
	dezalocareArboreDeMasini(&arbore);

	return 0;
}
