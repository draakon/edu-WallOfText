/* WallOfText.h :: Staatiline teek Wall Of Text teenustele: sisaldab põhiliselt tüükaid.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText_h.h"
#include <stdio.h>

extern unsigned char *uuid;

__inline void rpcError(char *function, RPC_STATUS errorCode);
__inline void checkAgainstBuffer(unsigned int bSize, char *input);

__inline void rpcError(char *function, RPC_STATUS errorCode) {
	unsigned char errorBuffer[256];

	DceErrorInqText(errorCode, errorBuffer);
	fprintf(stderr, "[-] (%s) Fataalne viga funktsioonis/blokis %s (%ld): %s\n", 
		__FUNCTION__, function, errorCode, errorBuffer);
	exit(errorCode);
}

/* Kontrollib. kas sisend mahub buffrisse,
 * mittemahtumine fataalne viga! */
__inline void checkAgainstBuffer(unsigned int bSize, char *input) {
	if (strlen(input)+1 > bSize) {
		fprintf(stderr,	"(!!!) Liiga suur argument!\n");
		exit(-1);
	}
}
