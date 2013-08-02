/* WallOfText_RP.c :: Wall of Text serveri kaugprotseduurid funktsioonid.
 * Autor: Kristjan Kaitsa
 * Hajuss�steemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText_RP.h"

// Lukud (l�imumisele)
HANDLE mAddMessage;

// Ping-Pong kontroll
unsigned char ping(unsigned char testValue) {
	return (testValue < UCHAR_MAX) ? testValue+1 : UCHAR_MAX;
}

/* Kirjutab outMessage pointerisse vastava IDga s�numi.
 * Kui sellist s�numit ei eksisteeri, siis tagastab -1. */
int getMessage(unsigned int messageId, pMessage outMessage) {
	Message *message = (Message*) dArray_get(database, messageId);
	unsigned long requestId = getAndIncCounter();

	printf("[%d] (%s) Klient tellib s�numi IDga: %u.\n", requestId, __FUNCTION__, messageId);
	if (message == NULL) {
		fprintf(stderr, "[%d] (%s) VIGA! Illegaalne ID.\n", requestId,  __FUNCTION__);
		return -1;
	}
	else {
		printf("[%d] (%s) V�ljastan kliendile s�numi.\n", requestId, __FUNCTION__);
		/* Kuna outMessages olevad embedded pointerid on NULLid, 
		 * siis eraldab t��gas ise kliendile m�lu. */
		if (!outMessage->data)
			outMessage->data = message->data;
		if (!outMessage->author)
			outMessage->author = message->author;
		if (!outMessage->time)
			outMessage->time = message->time;

		return 0;
	}
}

// Tagastab kliendi poolt m��ratud arvu viimased s�numeid.
unsigned int getLastMessages(unsigned int n, pMessages messages) {
	unsigned int i;
	unsigned int j = 0;
	pMessage message; 
	unsigned long requestId = getAndIncCounter();

	printf("[%d] (%s) Klient tellib viimased %u s�numi(t).\n", requestId, __FUNCTION__, n);

	if (database->size == 0) return 0;
	for (i = database->size-1; (j < n) && (i >= 0); i--) {
		message = (Message*) dArray_get(database, i);
		
		if (message != NULL) {
			printf("[%d] (%s) Lisan kliendile saatmiseks s�numi %u (indeks %u).\n",
				requestId, __FUNCTION__, i, j);
			messages[j++] = message;
		}
		if (i == 0) break;
	}
	return j;
}

// Lisab s�numi andmebaasi l�ppu.
void addMessage(pMessage newMessage) {
	unsigned long requestId = getAndIncCounter();

	// Ootame luku taga, kui vabaneb lukustame enda j�rel
	WaitForSingleObject(mAddMessage, INFINITE);

	printf("[%d] (%s) Klient postitas s�numi.\n", requestId, __FUNCTION__);
	dArray_append(database, newMessage);
	//Sleep(5000); // Luku testimiseks
	
	// Vabastame luku
	ReleaseMutex(mAddMessage);
}

