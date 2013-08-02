/* WallOfText_Server.c :: Teadetetahvli (Wall of Text) server kasutades
 * MS-RPC (DCE RPC) tehnoloogiat.
 * Autor: Kristjan Kaitsa
 * Hajuss�steemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText_Server.h"

unsigned long requestCounter = 0;				// P�ringute loendur
HANDLE mCounter;								// requestCounteri lukk

/* Vaikimisi konfiguratsioon;
 * v�ib siin muuta v�i k�ivitamisel v�tmeid kasutada (abiinfo -h).*/
char listenOnPort[B_PORT_SIZE]		= "1212";				// Port, kus kuulama asutakse (-p)
char databaseFile[B_DATABASE_SIZE]	= "andmebaas.dat";		// Andmebaas, mis laetakse (-d)

// Privaatsed protot��bid
RPC_STATUS CALLBACK callback_allow_everyone(__in  RPC_IF_HANDLE Interface, __in  void *Context);
void printHelp(void);
DWORD WINAPI threadListener(LPVOID lpParam);
__inline void threadError(char *msg);


int main(int argc, char *argv[]) {
	int i, errCode;
	char charBuffer;				// Klaviatuurilt t�htede salvestamiseks
	RPC_STATUS status;				// RPC funktsioonide tagastusv��rtuse jaoks
	HANDLE threadHandle;			// Kuulamisfunktsiooin poole p��rduva l�ime pide
	DWORD thereadId = 0;			// -""- id

	// Algv��rtustame lukud
	mAddMessage = CreateMutex(NULL, FALSE, NULL);
	mCounter = CreateMutex(NULL, FALSE, NULL);

	// �ritame lokalisatsiooni t�pit�htede jaoks paika panna
	if (!setlocale(LC_CTYPE, "est")) {
		fprintf(stderr,	"[-] (%s) HOIATUS! Teie systeemis ei 6nnestunud lokalisatsiooni seadistada."
			"T2pit2htede kuvamine v6ib eba6nnestuda.\n", __FUNCTION__);
	}

	// Informatsioon autori kohta
	puts("Wall of Text server v0.1\tKristjan Kaitsa\t\t2011 A.D.\n");

	// Sisendparameetrite t��tlemine
	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '/') || (argv[i][0] == '-') || (argv[i][0] == '\\')) {
			if (sizeof(argv[i]) < 2) continue;
			switch (tolower(argv[i][1])) {
			case 'p':	// pordi m��ramine
				if (i+1 >= argc) {
					fprintf(stderr, "Viga sisendargumentide t��tlemisel!\n");
					return -1;
				}
				checkAgainstBuffer(B_PORT_SIZE, argv[i+1]);
				strcpy(listenOnPort, argv[++i]);
				break;
			case 'd':	// andmebaasifaili m��ramine
				if (i+1 >= argc) {
					fprintf(stderr, "Viga sisendargumentide t��tlemisel!\n");
					return -1;
				}
				checkAgainstBuffer(B_DATABASE_SIZE, argv[i+1]);
				strcpy(databaseFile, argv[++i]);
				break;
			case 'h':	// abiinfo kuvamine
				printHelp();
				return -1;
				break;
			}
		}
	}
	
	// Andmebaasifaili laadimine
	while (errCode = loadDatabase()) {
		charBuffer = '\0';
		if (errCode == -2) {
			while ((charBuffer != 'j') && (charBuffer != 'e')) {
				fprintf(stderr, "[-] (%s) V�imalik, et antud faili ei eksisteeri. Kas �ritan luua [j/e]? ", 
					__FUNCTION__);
				charBuffer = tolower(getche());
				puts("");
			}
			if (charBuffer == 'j') {
				createBlankDatabase();
				continue;
			}
		}
		printf("[-] (%s) Sisestage alternatiivse andmebaasi nimi v�i vajutage v�ljumiseks ENTERit: ", 
			__FUNCTION__);
		fgets(databaseFile, B_DATABASE_SIZE, stdin);
		fflush(stdin);
		// Eemaldame reavahetuse
		databaseFile[strlen(databaseFile)-1] = '\0';
		if (databaseFile[0] == '\0')
			exit(-1);
	}

	// Konfigureerime protokolli
	status = RpcServerUseProtseqEp(
		(unsigned char*) "ncacn_ip_tcp",	// M��rame protokolliks TCP/IP
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT,		// "Backlog" j�rjekorra pikkus
		(unsigned char*) listenOnPort,		// TCP/IP port
		NULL
	);
	if (status)
		rpcError("RpcServerUseProtseqEp", status);

	// Registreerime teenuse liidese RPCga
	status = RpcServerRegisterIfEx(
		WallOfText_v1_0_s_ifspec,				// MIDL-i genereeritud liides
		NULL,
		NULL,
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,	/* Alates Windows XP SP2st vajalik vaikimisi
												 * RPC turvaseadistuse puhul, et lubada �hendusi.
												 * + vaja callback funktsiooni. */
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,
		&callback_allow_everyone				// Callback funktsioon, mis otsustab, kas lubada kliendil �henduda.
	);
	if (status)
		rpcError("RpcServerRegisterIfEx", status);

	// Loome eraldi l�ime, kuna tahame seda j�tta kasutajaliidese jaoks
	printf("[-] (%s) Loon kasutajaliidesest eraldiseisva l�ime.\n", __FUNCTION__);

	threadHandle = CreateThread(
		NULL,					// Vaikimisi turvaseaded
		0,                      // Vaikimisi pinu suurus
		threadListener,			// L�imefunktsiooni nimi
		NULL,					// Sisendit l�imele ei ole
		0,                      // Vaikimisi lipud
        &thereadId				// Salvestame l�ime ID
	);
	if (threadHandle == NULL)
		threadError("Fataalne viga l�ime loomisel; veakood");

	printf("[-] (%s) Serveri sulgemiseks vajutage [ESC]-klahvi.\n", __FUNCTION__);

	// Ootame kuni ESC-klahvi vajutatakse
	while (getch() != 27);

	// Alustame sulgemisega
	printf("[-] (%s) Alustan serveri sulgemist...\n", __FUNCTION__);
	status = RpcMgmtStopServerListening(NULL);
	if (status)
		rpcError("RpcMgmtStopServerListening", status);
	saveDatabase();

	printf("[-] (%s) Alustan kuularl�ime sulgemist...\n", __FUNCTION__);
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);

	/*
	Siin v�iks m�lu vabastada (nt. andmebaasi poolt kasutusel olnud),
	kuid samas t�nap�eva operatsioonis�steemid koristavad programmi j�rel naguinii �ra.
	*/

	printf("[-] (%s) ...server suletud.\n", __FUNCTION__);

	return 0;
}

// Thread-safe lukuga funktsioon samaaegselt counteri v��rtuse tagastamiseks ning suurendamiseks.
unsigned long getAndIncCounter(void) {
	unsigned long value;
	WaitForSingleObject(mCounter, INFINITE);
	value = requestCounter++;
	ReleaseMutex(mCounter);
	return requestCounter;
}

// L�imefuktsioon �henduste kuulamise alustamiseks.
DWORD WINAPI threadListener(LPVOID lpParam) {
	RPC_STATUS status;

	printf("[-] (%s) Asun kuulama pordil %s.\n", __FUNCTION__, listenOnPort);
	status = RpcServerListen(
		1,									// Soovitatav minimaalne arv l�imesid
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,		// Palju samaaegseid kaugprotseduure lubame
		FALSE								/* P��rdume tagasi alles p�rast 'RpcMgmtStopServerListening' kutsumist
											 * ning k�igi pooleli olevate kaugprotseduuride l�petamist.*/
	);
	if (status)
		rpcError("RpcServerListen", status);

	return 0;
}

// Fataalne l�imeviga.
__inline void threadError(char *msg) {
	int errCode = GetLastError();

	fprintf(stderr, "[-] (%s) %s. Veakood: %d.\n", 
		__FUNCTION__, msg, errCode);
	exit(errCode);
}

// Abiinfo kasutamiseks (-h v�tmega)
void printHelp(void) {
	puts("MS-RPC (DCE RPC) teenuse Wall of Text server v0.1\n"
		"Salvestab kasutaja saadetud saadetud s�numid andmebaasi ning kuvab neid\n"
		"tellimisel.\n"
		"Sisendparameetrid:\n\n"
		"-h\t\t- abiinfo (see sama info).\n"
		"-p <port>\t- m��rab pordi, kus kuulama hakata (vaikimisi: 1212).\n"
		"-d <failinimi>\t- m��rab andmebaasifaili (vaikimisi: andmebaas.dat).\n");
}

// Kontrollib, kas lubada kliendil �henduda
RPC_STATUS CALLBACK callback_allow_everyone(__in  RPC_IF_HANDLE Interface, __in  void *Context) {
	return RPC_S_OK;	// Lubame k�igil
}

// M�luhalduse funktsioonid (n�utud t��ka poolt)
void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len) {
	void *p = malloc(len);
	if (p == NULL) {
			fprintf(stderr, "[-] (%s) FATAALNE! M�lu eraldamine eba�nnestus.\n", __FUNCTION__);
			exit(-1);
	}
    return p;
}
 
void __RPC_USER midl_user_free(void __RPC_FAR * ptr) {
    free(ptr);
}
