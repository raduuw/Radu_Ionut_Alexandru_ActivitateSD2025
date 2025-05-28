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
typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};



//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

//LISTA SIMPLA
void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*stiva);
	(*stiva) = nou;

}

Masina popStack(Nod** stiva) {
	if ((*stiva) != NULL) {
		Masina nou = (*stiva)->info;
		Nod* prev = (*stiva);
		(*stiva) = (*stiva)->next;
		free(prev);
		return nou;
	}

	Masina m;
	m.id = -1;
	return m;
}

unsigned char emptyStack(Nod* stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {

	Nod* stiva = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void afisareStivaLSI(Nod* stiva) {
	Nod* temp = stiva;
	while (temp != NULL) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while ((*stiva) != NULL) {
		Masina m = popStack(stiva);
		free(m.numeSofer);
		free(m.model);

	}
}

int stackSize(Nod* stiva) {
	int nr = 0;
	while (stiva != NULL) {
		nr++;
		stiva = stiva->next;
	}
	return nr;
}

//LISTA DUBLA
typedef struct NodDubluStiva NodDubluStiva;
typedef struct ListaDublaStiva ListaDublaStiva;

struct NodDubluStiva {
	Masina info;
	NodDubluStiva* next;
	NodDubluStiva* prev;
};

struct ListaDublaStiva {
	NodDubluStiva* prim; 
	NodDubluStiva* ultim;  
};

void pushStivaLD(ListaDublaStiva* stiva, Masina m) {
	NodDubluStiva* nou = malloc(sizeof(NodDubluStiva));
	nou->info = m;
	nou->next = NULL;
	nou->prev = stiva->ultim;

	if (stiva->ultim != NULL) {
		stiva->ultim->next = nou;
	}
	else {
		stiva->prim = nou;
	}
	stiva->ultim = nou;
}

Masina popStivaLD(ListaDublaStiva* stiva) {
	Masina m;
	m.id = -1;
	if (stiva->ultim != NULL) {
		m = stiva->ultim->info;
		NodDubluStiva* aux = stiva->ultim;
		if (stiva->ultim->prev != NULL) {
			stiva->ultim = stiva->ultim->prev;
			stiva->ultim->next = NULL;
		}
		else {
			stiva->prim = NULL;
			stiva->ultim = NULL;
		}
		free(aux);
	}
	return m;
}

void afisareStivaLDI(ListaDublaStiva* stiva) {
	NodDubluStiva* temp = stiva->prim;
	while (temp != NULL) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void dezalocareStivaLD(ListaDublaStiva* stiva) {
	while (stiva->ultim != NULL) {
		Masina m = popStivaLD(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

ListaDublaStiva citireStackLDDinFisier(const char* numeFisier) {
	ListaDublaStiva stiva;
	stiva.prim = NULL;
	stiva.ultim = NULL;

	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return stiva;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)
			pushStivaLD(&stiva, m);
	}
	fclose(f);
	return stiva;
}

//VECTOR
typedef struct {
	Masina* elemente; 
	int capacitate;   
	int top;          
} StivaVector;



StivaVector initializareStivaVector(int capacitate) {
	StivaVector stiva;
	stiva.capacitate = capacitate;
	stiva.top = -1;
	stiva.elemente = (Masina*)malloc(sizeof(Masina) * capacitate);
	return stiva;
}

void pushStivaVector(StivaVector* stiva, Masina m) {
	if (stiva->top < stiva->capacitate - 1) {
		stiva->top++;
		stiva->elemente[stiva->top] = m;
	}
	else {
		printf("Stiva este plină!\n");
	}
}

Masina popStivaVector(StivaVector* stiva) {
	Masina m;
	m.id = -1;
	if (stiva->top >= 0) {
		m = stiva->elemente[stiva->top];
		stiva->top--;
	}
	else {
		printf("Stiva este goală!\n");
	}
	return m;
}

StivaVector citireStackVectorDinFisier(const char* numeFisier, int capacitate) {
	StivaVector stiva = initializareStivaVector(capacitate);
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return stiva;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)
			pushStivaVector(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void afisareStivaVector(StivaVector* stiva) {
	for (int i = stiva->top; i >= 0; i--) {
		afisareMasina(stiva->elemente[i]);
	}
}

unsigned char esteGoalaStivaVector(StivaVector* stiva) {
	return stiva->top == -1;
}

void dezalocareStivaVector(StivaVector* stiva) {
	for (int i = 0; i <= stiva->top; i++) {
		free(stiva->elemente[i].model);
		free(stiva->elemente[i].numeSofer);
	}
	free(stiva->elemente);
	stiva->elemente = NULL;
	stiva->top = -1;
	stiva->capacitate = 0;
}



//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

//LISTA DUBLA

typedef struct NodDublu NodDublu;
typedef struct ListaDubla ListaDubla;

struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

struct ListaDubla {
	NodDublu* prim;
	NodDublu* ultim;
};
void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->prev = coada->ultim;
	nou->next = NULL;
	if (coada->ultim != NULL) {
		coada->ultim->next = nou;
	}
	else {
		coada->prim = nou;
	}
	coada->ultim = nou;
}

Masina dequeue(ListaDubla* lista) {
	Masina m;
	m.id = -1;
	if (lista->prim != NULL) {

		m = lista->prim->info;
		NodDublu* aux = lista->prim;
		if (lista->prim->next != NULL) {
			lista->prim = lista->prim->next;
			lista->prim->prev = NULL;

		}
		else {
			lista->prim = NULL;
			lista->ultim = NULL;
		}

		free(aux);

	}
	return m;

}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&lista, m);
	}
	fclose(f);
	return lista;
}

void afisareCoadaLDI(ListaDubla* coada) {
	NodDublu* temp = coada->prim;
	while (temp != NULL) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void dezalocareCoadaDeMasini(ListaDubla* lista) {
	NodDublu* aux = lista->ultim;
	while (aux) {
		NodDublu* temp = aux;
		aux = aux->prev;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}


//LISTA SIMPLA
typedef struct NodCoada {
	Masina info;
	struct NodCoada* next;
} NodCoada;

typedef struct {
	NodCoada* front;  
	NodCoada* rear;   
} CoadaLSI;

void initCoadaLSI(CoadaLSI* coada) {
	coada->front = NULL;
	coada->rear = NULL;
}

void enqueueLSI(CoadaLSI* coada, Masina m) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->info = m;
	nou->next = NULL;
	if (coada->rear == NULL) {
		coada->front = nou;
		coada->rear = nou;
	}
	else {
		coada->rear->next = nou;
		coada->rear = nou;
	}
}

Masina dequeueLSI(CoadaLSI* coada) {
	Masina m;
	m.id = -1; 
	if (coada->front != NULL) {
		NodCoada* aux = coada->front;
		m = aux->info;
		coada->front = aux->next;
		if (coada->front == NULL) {
			coada->rear = NULL;
		}
		free(aux);
	}
	return m;
}

unsigned char esteGoalaCoadaLSI(CoadaLSI* coada) {
	return coada->front == NULL;
}

CoadaLSI citireCoadaLSIDinFisier(const char* numeFisier) {
	CoadaLSI coada;
	initCoadaLSI(&coada);

	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return coada;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)
			enqueueLSI(&coada, m);
	}
	fclose(f);
	return coada;
}

void afisareCoadaLSI(CoadaLSI* coada) {
	NodCoada* temp = coada->front;
	while (temp != NULL) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void dezalocareCoadaLSI(CoadaLSI* coada) {
	while (!esteGoalaCoadaLSI(coada)) {
		Masina m = dequeueLSI(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

//VECTOR
typedef struct {
	Masina* elemente;
	int capacitate;
	int front;   
	int rear;    
	int size;    
} CoadaVector;

CoadaVector initializareCoadaVector(int capacitate) {
	CoadaVector coada;
	coada.capacitate = capacitate;
	coada.front = 0;
	coada.rear = 0;
	coada.size = 0;
	coada.elemente = (Masina*)malloc(sizeof(Masina) * capacitate);
	return coada;
}

unsigned char esteGoalaCoadaVector(CoadaVector* coada) {
	return coada->size == 0;
}

unsigned char estePlinaCoadaVector(CoadaVector* coada) {
	return coada->size == coada->capacitate;
}

void enqueueVector(CoadaVector* coada, Masina m) {
	if (estePlinaCoadaVector(coada)) {
		printf("Coada este plina!\n");
		return;
	}
	coada->elemente[coada->rear] = m;
	coada->rear = (coada->rear + 1) % coada->capacitate;
	coada->size++;
}

Masina dequeueVector(CoadaVector* coada) {
	Masina m;
	m.id = -1;
	if (!esteGoalaCoadaVector(coada)) {
		m = coada->elemente[coada->front];
		coada->front = (coada->front + 1) % coada->capacitate;
		coada->size--;
	}
	else {
		printf("Coada este goala!\n");
	}
	return m;
}

CoadaVector citireCoadaVectorDinFisier(const char* numeFisier, int capacitate) {
	CoadaVector coada = initializareCoadaVector(capacitate);
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return coada;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)
			enqueueVector(&coada, m);
	}
	fclose(f);
	return coada;
}

void afisareCoadaVector(CoadaVector* coada) {
	for (int i = 0; i < coada->size; i++) {
		int idx = (coada->front + i) % coada->capacitate;
		afisareMasina(coada->elemente[idx]);
	}
}

void dezalocareCoadaVector(CoadaVector* coada) {
	for (int i = 0; i < coada->size; i++) {
		int idx = (coada->front + i) % coada->capacitate;
		free(coada->elemente[idx].model);
		free(coada->elemente[idx].numeSofer);
	}
	free(coada->elemente);
	coada->elemente = NULL;
	coada->capacitate = 0;
	coada->front = 0;
	coada->rear = 0;
	coada->size = 0;
}

//metode de procesare
Masina getMasinaByID(/*stiva sau coada de masini*/int id);

Masina* cautaMasinaDupaID_LSI(Nod* stiva, int id) {
	while (stiva) {
		if (stiva->info.id == id)
			return &(stiva->info);
		stiva = stiva->next;
	}
	return NULL;
}

Masina* cautaMasinaDupaID_LDI_Stiva(ListaDublaStiva* stiva, int id) {
	NodDubluStiva* current = stiva->prim;
	while (current) {
		if (current->info.id == id)
			return &(current->info);
		current = current->next;
	}
	return NULL;
}

Masina* cautaMasinaDupaID_VectorStiva(StivaVector* stiva, int id) {
	for (int i = 0; i <= stiva->top; i++) {
		if (stiva->elemente[i].id == id)
			return &(stiva->elemente[i]);
	}
	return NULL;
}

Masina* cautaMasinaDupaID_LSI_Coada(CoadaLSI* coada, int id) {
	Nod* current = coada->front;
	while (current != NULL) {
		if (current->info.id == id) {
			return &(current->info);
		}
		current = current->next;
	}
	return NULL;
}

Masina* cautaMasinaDupaID_LDI_Coada(ListaDubla* coada, int id) {
	NodDublu* current = coada->prim;
	while (current) {
		if (current->info.id == id)
			return &(current->info);
		current = current->next;
	}
	return NULL;
}

Masina* cautaMasinaDupaID_VectorCoada(CoadaVector* coada, int id) {
	for (int i = 0; i < coada->size; i++) {
		int idx = (coada->front + i) % coada->capacitate;
		if (coada->elemente[idx].id == id)
			return &(coada->elemente[idx]);
	}
	return NULL;
}

float calculeazaPretTotal(ListaDubla* coada) {
	float suma = 0;
	ListaDubla nou;
	nou.prim = NULL;
	nou.ultim = NULL;
	while ((*coada).prim) {
		Masina m = dequeue(coada);
		suma += m.pret;
		enqueue(&nou, m);

	}
	coada->prim = nou.prim;
	coada->ultim = nou.ultim;

	return suma;
}


int modificaPretStivaLSI(Nod* stiva, int id, float nouPret) {
	while (stiva) {
		if (stiva->info.id == id) {
			stiva->info.pret = nouPret;
			return 1;
		}
		stiva = stiva->next;
	}
	return 0;
}

int modificaPretStivaLDI(ListaDublaStiva* stiva, int id, float nouPret) {
	NodDubluStiva* current = stiva->prim;
	while (current) {
		if (current->info.id == id) {
			current->info.pret = nouPret;
			return 1;
		}
		current = current->next;
	}
	return 0;
}

int modificaPretStivaVector(StivaVector* stiva, int id, float nouPret) {
	for (int i = 0; i <= stiva->top; i++) {
		if (stiva->elemente[i].id == id) {
			stiva->elemente[i].pret = nouPret;
			return 1;
		}
	}
	return 0;
}


int modificaPretCoadaLSI(CoadaLSI* coada, int id, float nouPret) {
	Nod* current = coada->front;
	while (current) {
		if (current->info.id == id) {
			current->info.pret = nouPret;
			return 1;
		}
		current = current->next;
	}
	return 0;
}

int modificaPretCoadaLDI(ListaDubla* coada, int id, float nouPret) {
	NodDublu* current = coada->prim;
	while (current) {
		if (current->info.id == id) {
			current->info.pret = nouPret;
			return 1;
		}
		current = current->next;
	}
	return 0;
}

int modificaPretCoadaVector(CoadaVector* coada, int id, float nouPret) {
	for (int i = 0; i < coada->size; i++) {
		int idx = (coada->front + i) % coada->capacitate;
		if (coada->elemente[idx].id == id) {
			coada->elemente[idx].pret = nouPret;
			return 1;
		}
	}
	return 0;
}

int main() {

	Nod* stivaLSI = citireStackMasiniDinFisier("masini.txt");
	printf("Stiva (lista simpla) - Nr elemente: %d\n", stackSize(stivaLSI));

	Masina m = popStack(&stivaLSI);
	printf("Element scos din stiva (LSI):\n");
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);

	dezalocareStivaDeMasini(&stivaLSI);

	CoadaLSI coadaLSI = citireCoadaLSIDinFisier("masini.txt");
	printf("Coada (lista simpla)\n");

	m = dequeueLSI(&coadaLSI);
	printf("Element scos din coada (LSI):\n");
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);


	dezalocareCoadaLSI(&coadaLSI);

	return 0;


	/*ListaDublaStiva stivaLD = citireStackLDDinFisier("masini.txt");

	Masina m = popStivaLD(&stivaLD);
	printf("Element scos din stiva (lista dubla):\n");
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);

	dezalocareStivaLD(&stivaLD);

	ListaDubla coadaLD = citireCoadaDeMasiniDinFisier("masini.txt");

	m = dequeue(&coadaLD);
	printf("Element scos din coada (lista dubla):\n");
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);

	dezalocareCoadaDeMasini(&coadaLD);

	return 0;

	StivaVector stivaVec = citireStackVectorDinFisier("masini.txt", 100);

	Masina m = popStivaVector(&stivaVec);
	printf("Element scos din stiva (vector):\n");
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);

	dezalocareStivaVector(&stivaVec);

	CoadaVector coadaVec = citireCoadaVectorDinFisier("masini.txt", 100);

	m = dequeueVector(&coadaVec);
	printf("Element scos din coada (vector):\n");
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);

	dezalocareCoadaVector(&coadaVec);

	return 0;*/
}