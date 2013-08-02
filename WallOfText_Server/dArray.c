/* dArray.c :: Dünaamiline massiiv array'de otsas.
 * Autor: Kristjan Kaitsa
 * 2011 A.D. */
#include "dArray.h"

/* Etteantud vahemiku viidete NULLimine:
 * start - kaasaarvatud; length - välja arvatud */
void dArray_init(dArray *arr, unsigned int start, unsigned int length) {
	static unsigned int i;
#ifdef DEBUG
	printf("Algväärtustan indeksite vahemiku %d-%d.\n", start, length-1);
#endif DEBUG
	for (i = start; i < length; i++)
		arr->data[i] = NULL;
}

/* Kui nõutud suurus on praegusest massiivi mahust suurem, siis
 * suurendatakse seda kahega korrutades kuni massiiv on vähemalt nii suur. */
void dArray_extendIfNeeded(dArray *arr, unsigned int newSize) {
	unsigned int oldSize = arr->capacity;
	if (newSize > arr->capacity) {
		// ceil(log2(oodatav suurus/praegune suurus))=mitu sammu vasakule
		int power = (int)ceil(log((float)newSize/(float)arr->capacity)/log((float)2));
		arr->capacity <<= power; // Suurendame massiivi kahendastme võrra
#ifdef DEBUG
		printf("Kahendaste: %d. Suurendame massiivi. Uus suurus: %d.\n", power, arr->capacity);
#endif DEBUG
		arr->data = (void**) realloc(arr->data, sizeof(int)*arr->capacity);

		dArray_init(arr, oldSize, arr->capacity);
	}
}

// Uue dünaamilise massiivi loomine
dArray* dArray_new(unsigned int capacity) {
	dArray *arr = (dArray*) malloc(sizeof(dArray));

	arr->capacity = capacity;
	arr->size = 0;
	arr->data = (void**) malloc(sizeof(void*)*arr->capacity);
	dArray_init(arr, 0, arr->capacity);

	return arr;
}

// Kirje lisamine massiivi lõppu
unsigned int dArray_append(dArray *arr, void *data) {
	unsigned int oldSize = arr->capacity;
	if (arr->size == arr->capacity) {
		arr->capacity <<= 1; // Suurendame massiivi kaks korda
#ifdef DEBUG
		printf("Suurendame massiivi. Uus suurus: %d\n", arr->capacity);
#endif DEBUG
		arr->data = (void**) realloc(arr->data, sizeof(int)*arr->capacity);
		dArray_init(arr, oldSize, arr->capacity);
	}
	arr->data[arr->size++] = data;

	return arr->size - 1;
}

// Muudab kasutusel olevate kohtade arvu; reaalselt midagi alusmassiivist ei kustutata.
void dArray_truncateBy(dArray *arr, unsigned int removeFromTail) {
	unsigned int newSize = arr->size - removeFromTail;
	arr->size = (newSize < 0) ? 0 : newSize;
}

// Viimane element tagastatakse ning "kustutatakse".
void* dArray_popLast(dArray* arr) {
	if (arr->size > 0)
		return arr->data[--arr->size];
	else 
		return NULL;
}

/* Väärtuse seadmine soovitud indeksile, vajadusel suurendades massiivi.
 * NB! Kui lisatakse size'st kaugemale, siis jäävad vahepeale NULLitud viidad. */
void dArray_set(dArray *arr, unsigned int index, void *data) {
	dArray_extendIfNeeded(arr, index+1);
	arr->data[index] = data;
	if (arr->size <= index) arr->size = index+1;
}

/* Indeksilt väärtuse tagastamine. Kui massiiv on väiksem nõutud indeksist, siis
 * antakse NULL. */
void* dArray_get(dArray *arr, unsigned int index) {
	if ((index >= arr->capacity) || (index >= arr->size)) return NULL;
	else return arr->data[index];
}

void dArray_swap(dArray *arr, int index1, int index2) {
	void* tmp = arr->data[index1];
	
#ifdef DEBUG
	printf("Vahetan viited indeksitel %d ja %d.\n", index1, index2);
#endif DEBUG
	arr->data[index1] = arr->data[index2];
	arr->data[index2] = tmp;	
}
