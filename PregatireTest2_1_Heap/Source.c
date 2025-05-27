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

struct Heap {
	int lungime;
	Masina* vector;
	int nrMasini;  
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

//Max Heap
void filtreazaMaxHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozFiuSt<heap.nrMasini && heap.vector[pozMax].id < heap.vector[pozFiuSt].id) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr<heap.nrMasini && heap.vector[pozMax].id < heap.vector[pozFiuDr].id) {
		pozMax = pozFiuDr; 
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrMasini - 2) / 2) {
			filtreazaMaxHeap(heap, pozMax);
		}
	}

}

//Min Heap
void filtreazaMinHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMin = pozitieNod;
	if (pozFiuSt < heap.nrMasini && heap.vector[pozMin].id > heap.vector[pozFiuSt].id) {
		pozMin = pozFiuSt;
	}
	if (pozFiuDr < heap.nrMasini && heap.vector[pozMin].id > heap.vector[pozFiuDr].id) {
		pozMin = pozFiuDr;
	}
	if (pozMin != pozitieNod) {
		Masina aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMin <= (heap.nrMasini - 2) / 2) {
			filtreazaMinHeap(heap, pozMin);
		}
	}
}

Heap citireMaxHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrMasini++] = citireMasinaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrMasini - 2) / 2;i >= 0;i--) {
		filtreazaMaxHeap(heap, i);
	}
	return heap;
}

Heap citireMinHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrMasini++] = citireMasinaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrMasini - 2) / 2;i >= 0;i--) {
		filtreazaMinHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0;i < heap.nrMasini;i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini;i < heap.lungime;i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasinaMaxHeap(Heap* heap) {
	
	if (heap->nrMasini>0) {
		Masina aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrMasini - 1];
		heap->vector[heap->nrMasini - 1] = aux;
		heap->nrMasini--;
		for (int i = (heap->nrMasini - 2) / 2;i >= 0;i--) {
			filtreazaMaxHeap(*heap, i);
		}
		return aux;

	}
}

Masina extrageMasinaMinHeap(Heap* heap) {

	if (heap->nrMasini > 0) {
		Masina aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrMasini - 1];
		heap->vector[heap->nrMasini - 1] = aux;
		heap->nrMasini--;
		for (int i = (heap->nrMasini - 2) / 2;i >= 0;i--) {
			filtreazaMinHeap(*heap, i);
		}
		return aux;
	}
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0;i < heap->lungime;i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;

}

void cautaMasinaDupaModelMaxHeap(Heap heap, const char* model) {
	printf("\nMasini cu modelul '%s':\n", model);
	for (int i = 0; i < heap.nrMasini; i++) {
		if (strcmp(heap.vector[i].model, model) == 0) {
			afisareMasina(heap.vector[i]);
		}
	}
}

void modificaPretMasinaDupaIDMaxHeap(Heap* heap, int id, float pretNou) {
	for (int i = 0; i < heap->nrMasini; i++) {
		if (heap->vector[i].id == id) {
			heap->vector[i].pret = pretNou;
			printf("Pret modificat pentru masina cu ID %d.\n", id);
			for (int j = (heap->nrMasini - 2) / 2; j >= 0; j--) {
				filtreazaMaxHeap(*heap, j);
			}
			return;
		}
	}
	printf("Masina cu ID %d nu a fost gasita.\n", id);
}

void extrageMasiniCuIdMaiMareMaxHeap(Heap* heap, int pragID) {
	printf("\nMasini extrase cu ID mai mare decat %d:\n", pragID);
	int i = 0;
	while (i < heap->nrMasini) {
		if (heap->vector[i].id > pragID) {
			afisareMasina(heap->vector[i]);
			Masina extras = heap->vector[i];
			heap->nrMasini--;
			heap->vector[i] = heap->vector[heap->nrMasini]; 
			heap->vector[heap->nrMasini] = extras;         

			for (int j = (heap->nrMasini - 2) / 2; j >= 0; j--) {
				filtreazaMaxHeap(*heap, j);
			}
		}
		else {
			i++;
		}
	}
}


int main() {
	Heap maxHeap = citireMaxHeapDeMasiniDinFisier("masini.txt");
	printf("Max Heap:\n");
	afisareHeap(maxHeap);

	cautaMasinaDupaModelMaxHeap(maxHeap, "Logan");
	modificaPretMasinaDupaIDMaxHeap(&maxHeap, 7, 30000.0f);
	printf("\nDupa modificarea pretului:\n");
	afisareHeap(maxHeap);

	extrageMasiniCuIdMaiMareMaxHeap(&maxHeap, 5);
	printf("\nDupa extrageri:\n");
	afisareHeap(maxHeap);

	printf("\nMasini ascunse (extrase):\n");
	afiseazaHeapAscuns(maxHeap);

	dezalocareHeap(&maxHeap);
	return 0;
}