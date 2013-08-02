/* WallOfText_Database.c :: Wall of Text serveri andmebaasi hoidmine, 
 * laadimine, salvestamine.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText_Database.h"

char saveFileIdentifier[] = {'W', 'o', 'T'};	// "Maagiline number".
unsigned char saveFileVersion = 1;				// Andmebaasifaili versioon
dArray *database;								// Sõnumite/teadete andmebaas

// Privaatsed prototüübid
void writeDatabaseHeader(FILE *file);

void writeDatabaseHeader(FILE *file) {
	unsigned int count = 0;

	// Faili identifitseerimismärge (magic number)
	fwrite(&saveFileIdentifier, sizeof(char), sizeof(saveFileIdentifier), file);
	// Faili struktuuri versioon (tulevaste versioonidega ühilduvuse jaoks ;))
	fwrite(&saveFileVersion, sizeof(saveFileVersion), 1, file);		
	// Kirjete arv andmebaasis (väärtustatakse hiljem)
	fwrite(&count, sizeof(count), 1, file);
}

int createBlankDatabase(void) {
	FILE *file = fopen(databaseFile, "wb");
	
	if (file == NULL) {
		fprintf(stderr, "[-] (%s) Ei õnnestunud faili '%s' luua!\n", __FUNCTION__, databaseFile);
		return -1;
	}

	writeDatabaseHeader(file);

	fclose(file);
	return 0;
}

// Laeb binaarfailist andmebaasi.
int loadDatabase(void) {
	FILE *file = fopen(databaseFile, "rb");
	char fileStart[3];
	unsigned char version;
	unsigned char authorLen;
	unsigned short dataLen;
	unsigned int countReported;
	unsigned int count;
	Message *message;

	printf("[-] (%s) Asun andmebaasifaili '%s' laadima.\n", __FUNCTION__, databaseFile);
	if (file == NULL) {
		fprintf(stderr, "[-] (%s) Ei õnnestunud andmebaasifaili '%s' lugemiseks avada!\n", __FUNCTION__, databaseFile);
		return -2;
	}

	// Kontrollime "maagilist numbrit" (identifikaatorit)
	version = fread(&fileStart, sizeof(char), 3, file);
	if (memcmp(fileStart, saveFileIdentifier, 3)) {
		fprintf(stderr, "[-] (%s) Vigane andmebaasifail.\n", __FUNCTION__);
		fclose(file);
		return -1;
	}

	// Kontrollime versiooni
	version = fread(&version, sizeof(saveFileVersion), 1, file);
	if (version != 1) {
		fprintf(stderr, "[-] (%s) Sobimatu andmebaasiversioon: %ho\n", __FUNCTION__, version);
		fclose(file);
		return -1;
	}

	// Loeme sisse kirjete arvu
	fread(&countReported, sizeof(countReported), 1, file);
	printf("[-] (%s) Andmebaasifailis peaks olema %u kirjet.\n", __FUNCTION__, countReported);

	database = dArray_new(countReported > 0 ? countReported : 16);

	for (count = 0; count < countReported; count++) {
		message = (Message*) malloc(sizeof(Message));
		if (message == NULL) {
			fprintf(stderr, "[-] (%s) FATAALNE! Mälu eraldamine ebaõnnestus.\n", __FUNCTION__);
			exit(-1);
		}
		// Autori nime pikkus
		fread(&authorLen, sizeof(authorLen), 1, file);
		// Autori nimi
		message->author = (char*) malloc(sizeof(char) * authorLen + 1);
		if (message->author == NULL) {
			fprintf(stderr, "[-] (%s) FATAALNE! Mälu eraldamine ebaõnnestus.\n", __FUNCTION__);
			exit(-1);
		}
		memset(message->author, '\0', authorLen + 1);
		fread(message->author, sizeof(char), authorLen, file);
		// Sõnumi kellaaeg
		fread(&message->time, sizeof(time_t), 1, file);
		// Sõnumi sisu pikkus
		fread(&dataLen, sizeof(dataLen), 1, file);
		// Sõnumi sisu
		message->data = (char*) malloc(sizeof(char) * dataLen + 1);
		if (message->data == NULL) {
			fprintf(stderr, "[-] (%s) FATAALNE! Mälu eraldamine ebaõnnestus.\n", __FUNCTION__);
			exit(-1);
		}
		memset(message->data, '\0', dataLen + 1);
		fread(message->data, sizeof(char), dataLen, file);

		if (feof(file) || ferror(file)) {
			fprintf(stderr, "[-] (%s) Failist lugemisel tekkis tõrge. Lõpetan kirjete sisselugemise.\n", __FUNCTION__);
			if (message != NULL) {
				if (message->author != NULL) free(message->author);
				if (message->data != NULL) free(message->data);
				free(message);
			}
		}
		dArray_append(database, message);
	}
	fclose(file);
	printf("[-] (%s) %u andmebaasikirjet laetud.\n", __FUNCTION__, count);
	return 0;
}

/* Salvestab andmebaasi binaarkujul faili.
 * Ei pruugi olla porditav teistele platvormidele:
 * muutujate suurused, big endian vs little endian. */
void saveDatabase(void) {
	FILE *file = fopen(databaseFile, "wb");
	unsigned int i;
	unsigned int attributeLen;
	unsigned char authorLen;
	unsigned short dataLen;
	unsigned int count = 0;
	Message *message;

	if (file == NULL) {
		fprintf(stderr, "[-] (%s) VIGA! Ei õnnestunud andmebaasifaili '%s' salvestamiseks avada!\n", __FUNCTION__, databaseFile);
		return;
	}
	printf("[-] (%s) Asun andmebaasi faili '%s' salvestama.\n", __FUNCTION__, databaseFile);

	writeDatabaseHeader(file);

	for (i = 0; i < database->size; i++) {
		message = (Message*) database->data[i];
		if (message == NULL) continue;

		// Autori nimi
		// Teksti pikkus (0-255)
		attributeLen = strlen(message->author);
		authorLen = (attributeLen > UCHAR_MAX) ? UCHAR_MAX : attributeLen;
		
		fwrite(&authorLen, sizeof(authorLen), 1, file);

		// Tekst ilma \0 terminaatorita
		fwrite(message->author, sizeof(char), authorLen, file);

		// Sõnumi kellaeg
		fwrite(&message->time, sizeof(time_t), 1, file);

		// Sõnumi sisu
		// Teksti pikkus (0-255)
		attributeLen = strlen(message->data);
		dataLen = (attributeLen > ULONG_MAX) ? ULONG_MAX : attributeLen;
		fwrite(&dataLen, sizeof(dataLen), 1, file);

		// Tekst ilma \0 terminaatorita
		fwrite(message->data, sizeof(char),	dataLen, file);

		count++;
	}
	// Kirjutame kirjete arvu
	fseek(file, sizeof(saveFileVersion)+sizeof(char)*sizeof(saveFileIdentifier), SEEK_SET);
	fwrite(&count, sizeof(count), 1, file);
	
	fclose(file);
	printf("[-] (%s) %u kirje(t) andmebaasifaili salvestatud.\n", __FUNCTION__, count);
}