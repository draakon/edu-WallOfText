/* WallOfText_Client.c :: MS-RPC (DCE RPC) tehnoloogiat kasutava
 * teadetetahvli (Wall of Text) serveri klient.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText_Client.h"

/* Vaikimisi konfiguratsioon;
 * võib siin muuta või käivitamisel võtmeid kasutada (abiinfo -h).*/
char serverHost[B_HOSTNAME_SIZE]	= "localhost";		// Serveri aadress (-s)
char serverPort[B_PORT_SIZE]		= "1212";			// Sereri port (-p)

// Privaatsed prototüübid
void getAndPrintMessage(unsigned int messageId);
void getAndPrintMessages(unsigned int n);
void printMessage(Message *message);
void printMessages(Message **messages, unsigned int n);
void printHelp(void);
void freeMessagesMemory(Message **messages, unsigned int n);
int sendNewMessage(char *author);
char* gatherDynamicText(void);
__inline void argumentError(void);

typedef enum tagProcedure {
	NONE,
	ADD_NEW,
	GET,
	GET_LAST
} Procedure;

int main(int argc, char **argv) {
	int i;
	unsigned char pingCheck;				// Ping-Pong checki jaoks
	RPC_STATUS status;						// RPC funktsioonide tagastusväärtuse jaoks
	Procedure procedureToCall = NONE;		// Protseduur, mida klient peaks serverilt küsima
	char inputBuffer[B_INPUT_SIZE];			// Buffer argumendi jaoks
	unsigned char *pszStringBinding;		// Pidemesidumissõne (põhimõtteliselt seadistused)

	// Üritame lokalisatsiooni täpitähtede jaoks paika panna
	if (!setlocale(LC_CTYPE, "est")) {
		fprintf(stderr,	"(!!!) Teie systeemis ei 6nnestunud lokalisatsiooni seadistada. "
			"T2pit2htede kuvamine v6ib eba6nnestuda.\n\n");
	}

	// Informatsioon autori kohta
	puts("Wall of Text klient v0.1\tKristjan Kaitsa\t\t2011 A.D.\n");

	// Sisendparameetrite töötlemine
	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '/') || (argv[i][0] == '-') || (argv[i][0] == '\\')) {
			if (sizeof(argv[i]) < 2) continue;
			switch (tolower(argv[i][1])) {
			case 'p':	// pordi määramine
				if (i+1 >= argc)
					argumentError();
				checkAgainstBuffer(B_PORT_SIZE, argv[i+1]);
				strcpy(serverPort, argv[++i]);
				break;
			case 's':	// serveri aadressi määramine
				if (i+1 >= argc)
					argumentError();
				checkAgainstBuffer(B_HOSTNAME_SIZE, argv[i+1]);
				strcpy(serverHost, argv[++i]);
				break;
			case 'g':	// kirje pärimine id-järgi
				if ((i+1 >= argc) || (procedureToCall != NONE))
					argumentError();
				checkAgainstBuffer(B_INPUT_SIZE, argv[i+1]);
				procedureToCall = GET;
				strcpy(inputBuffer, argv[++i]);
				break;
			case 'l':	// viimase n-i kirje pärimine
				if (procedureToCall != NONE)
					argumentError();
				procedureToCall = GET_LAST;
				if ((i+1 >= argc) || (isdigit(argv[i+1][0]) == 0))
					strcpy(inputBuffer, "1");
				else {
					checkAgainstBuffer(B_INPUT_SIZE, argv[i+1]);
					strcpy(inputBuffer, argv[++i]);
				}
				break;
			case 'a':	// uue kirje lisamine
				if ((i+1 >= argc) || (procedureToCall != NONE))
					argumentError();
				checkAgainstBuffer(B_INPUT_SIZE, argv[i+1]);
				procedureToCall = ADD_NEW;
				strcpy(inputBuffer, argv[++i]);
				break;
			case 'h':	// abiinfo kuvamine
				printHelp();
				break;
			default:
				argumentError();
			}
		}
	}

	// Kontrollime, kas protseduur on määratud
	if (procedureToCall == NONE) {
		fprintf(stderr, "(!!!) Protseduur määramata [-a,-l,-g].\n\n");
		printHelp();
		return -1;
	}

	// Koostame "sidumissõne"
	status = RpcStringBindingCompose(
		uuid,								// UUID
		(unsigned char*) "ncacn_ip_tcp",	// protokolliks TCP/IP
		(unsigned char*) serverHost,		// Serveri aadress
		(unsigned char*) serverPort,		// Port
		NULL,								// Täiendavaid seadistusi pole
		&pszStringBinding					// Viit muutujale, kuhu salvestada
	);
	if (status)
		rpcError("RpcStringBindingCompose", status);

	// Teeme "sidumissõnest" "sidumispidemine" :)
	status = RpcBindingFromStringBinding(
		pszStringBinding,					// Viide stringile (sisend)
		&wallOfText_IfHandle				// Viide pidemele (väljund)
	);
	if (status)
		rpcError("RpcBindingFromStringBinding", status);

	// Vabastame "sidumissõne"
	RpcStringFree(&pszStringBinding);

	// Proovime serverile päringut teha
	RpcTryExcept {
		// Kontrollime, kas server on korras:
		pingCheck = rand() % UCHAR_MAX;
		if (ping(pingCheck) != pingCheck+1) {
			fprintf(stderr, "(!!!) Fataalne! Server ei vastanud ping-pongile korrektselt!\n");
			exit(-1);
		}

		// Teeme oma protseduurikutse
		switch (procedureToCall) {
		case GET:
			getAndPrintMessage(strtoul(inputBuffer, NULL, 0));
			break;
		case GET_LAST:
			getAndPrintMessages(strtoul(inputBuffer, NULL, 0));
			break;
		case ADD_NEW:
			if (sendNewMessage(inputBuffer))
				fprintf(stderr, "(!!!) Sõnumi lisamine ebaõnnestus!\n");
			else
				printf("\n\n--> Sõnum serverile saadetud!\n");
			break;
		}	
	} RpcExcept(1) {
		rpcError("RpcTryExcept", RpcExceptionCode());
	} RpcEndExcept

	// Vabastame pideme
	RpcBindingFree(&wallOfText_IfHandle);
	return 0;
}

/* Kogub dünaamiliselt tekstisisendit.
 * Peaks olema kaitstud bufferi ületäitumise eest.
 * Töötab ainult DOSis/Windowsis (getch()). */
char* gatherDynamicText(void) {
	unsigned int bufferSize = 15;
	unsigned int curLength = 0;
	char *inputBuffer = (char*) malloc(sizeof(char) * bufferSize);
	char nextChar;

	printf("Teksti sisestamise lõpetamiseks vajutage [ESC]:\n");
	while (nextChar = getch()) {
		if (nextChar == 27) break;		// [ESC]
		else if (nextChar == 13) {		// [ENTER]
			puts("");
			nextChar = '\n';
		}
		else if (nextChar == 8)	{		// [BACKSPACE]
			if (curLength > 0) {
				curLength--;
				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
			continue;
		}
		else putchar(nextChar);

		inputBuffer[curLength++] = nextChar;
		if (curLength >= bufferSize) {
			bufferSize += 15;
			inputBuffer = (char*) realloc(inputBuffer, bufferSize);
		}
	}
	inputBuffer[curLength] = '\0';

	return inputBuffer;
}

// Võtab kasutajalt uue sõnumi ning saadab serverisse.
int sendNewMessage(char *author) {
	Message newMessage;
	time_t rawtime;
	char *data;

	if (strlen(author) == 0) {
		fprintf(stderr, "\n\n(!!!) Sõnumi autor määramata!\n");
		return -1;
	}
	data = gatherDynamicText();
	if (strlen(data) == 0) {
		fprintf(stderr, "\n\n(!!!) Tühi sõnum ei ole lubatud!\n");
		return -1;
	}

	rawtime = time(NULL);
	newMessage.time = mktime(localtime(&rawtime));
	newMessage.author = author;
	newMessage.data = data;
	addMessage(&newMessage);

	free(newMessage.data);
	return 0;
}

// Visatakse, kui ei õnnestu sisendargumente parseda.
__inline void argumentError(void) {
	fprintf(stderr, "(!!!) Viga sisendargumentide töötlemisel!\n");
	printHelp();
	exit(-1);
}

// Pärib serverilt antud ID-ga sõnumit ning väljastab.
void getAndPrintMessage(unsigned int messageId) {
	Message message;

	memset(&message, '\0', sizeof(Message));
	if (getMessage(messageId, &message) != 0) {
		fprintf(stderr, "(!!!) Kirje %u pärimine ebaõnnestus. Tõenäoliselt seda ei eksisteeri.\n",
			messageId);
	}
	else printMessage(&message);
}

// Pärib serverilt n viimast sõnumit ning väljastab need.
void getAndPrintMessages(unsigned int n) {
	unsigned int messagesReturned;	// ei pruugi olla nii palju kui tahetakse
	Message **messages = (Message**) midl_user_allocate(sizeof(Message*)*n);

	if (n == 0) {
		fprintf(stderr, "(!!!) Serverit pole mõttet 0 kirje pärimisega tüüdata.\n");
		return;
	}
	memset(messages, '\0', sizeof(Message*)*n);
	
	messagesReturned = getLastMessages(n, messages);
	if (messagesReturned == 0)
		fprintf(stderr, "(!!!) Serveris ei ole ühtegi kirjet.\n");
	else {
		if (messagesReturned < n)
			fprintf(stderr, "(!!!) Server saatis vähem kirjeid kui küsiti. Rohkem ei ole?\n\n");
		printMessages(messages, messagesReturned);
	}

	freeMessagesMemory(messages, n);
}

// Vabastab Messages masiivi mälu
void freeMessagesMemory(Message **messages, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n; i++) {
		if (messages[i] == NULL) break;
		if (messages[i]->author != NULL)
			midl_user_free(messages[i]->author);
		if (messages[i]->data != NULL)
			midl_user_free(messages[i]->data);
		midl_user_free(messages[i]);
	}
	midl_user_free(messages);
}

// Vormistab ja väljastab sõnumi standardväljundisse
void printMessage(Message *message) {
	char *hrTime = NULL;
	
	if (message->time != 0) {
		hrTime = asctime(localtime(&message->time));
		// Kustutame lõpust reavahetuse
		hrTime[strlen(hrTime)-1] = '\0';
	}
	if (message->author != NULL)
		printf("-===( %s )===-\n", message->author);
	else
		puts("-===( Tundmatu )===-");
	if (hrTime != NULL)
		printf("Aeg: %s\n", hrTime);
	printf("Sisu: %s\n", message->data);
}

// Väljastab järjest kõik sõnumid massiivis.
void printMessages(Message **messages, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n; i++) {
		if (messages[i] == NULL) break;
		printMessage(messages[i]);
	}
}

// Abiinfo
void printHelp(void) {
	puts("MS-RPC (DCE RPC) teenuse Wall of Text klient v0.1\n"
		"Võimaldab kliendil serverist tellida sõnumeid ning enda omasid sinna lisada.\n"
		"Sisendparameetrid:\n\n"
		"-h\t\t- abiinfo (see sama info).\n"
		"-p <port>\t- määrab pordi, kuhu üritada ühenduda (vaikimisi: 1212).\n"
		"-s <aadress>\t- määrab serveri aadressi (vaikimisi: localhost).\n\n"
		"Määratud peab oleva üks järgnevatest protseduuridest:\n"
		"-g <id>\t\t- pärib <id>le vastavat sõnumit.\n"
		"-a <autor>\t- lisab uue sõnumi (sisu võetakse standardsisendist).\n"
		"-l [n]\t\t- tagastab viimased <n> sõnumit (vaikimisi: 1).");
}

// Mäluhalduse funktsioonid (nõutud tüüka poolt)
void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len) {
	void *p = malloc(len);
	if (p == NULL) {
			fprintf(stderr, "[-] (%s) FATAALNE! Mälu eraldamine ebaõnnestus.\n", __FUNCTION__);
			exit(-1);
	}
    return p;
}
 
void __RPC_USER midl_user_free(void __RPC_FAR * ptr) {
    free(ptr);
}