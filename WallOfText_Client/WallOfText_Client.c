/* WallOfText_Client.c :: MS-RPC (DCE RPC) tehnoloogiat kasutava
 * teadetetahvli (Wall of Text) serveri klient.
 * Autor: Kristjan Kaitsa
 * Hajuss�steemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText_Client.h"

/* Vaikimisi konfiguratsioon;
 * v�ib siin muuta v�i k�ivitamisel v�tmeid kasutada (abiinfo -h).*/
char serverHost[B_HOSTNAME_SIZE]	= "localhost";		// Serveri aadress (-s)
char serverPort[B_PORT_SIZE]		= "1212";			// Sereri port (-p)

// Privaatsed protot��bid
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
	RPC_STATUS status;						// RPC funktsioonide tagastusv��rtuse jaoks
	Procedure procedureToCall = NONE;		// Protseduur, mida klient peaks serverilt k�sima
	char inputBuffer[B_INPUT_SIZE];			// Buffer argumendi jaoks
	unsigned char *pszStringBinding;		// Pidemesidumiss�ne (p�him�tteliselt seadistused)

	// �ritame lokalisatsiooni t�pit�htede jaoks paika panna
	if (!setlocale(LC_CTYPE, "est")) {
		fprintf(stderr,	"(!!!) Teie systeemis ei 6nnestunud lokalisatsiooni seadistada. "
			"T2pit2htede kuvamine v6ib eba6nnestuda.\n\n");
	}

	// Informatsioon autori kohta
	puts("Wall of Text klient v0.1\tKristjan Kaitsa\t\t2011 A.D.\n");

	// Sisendparameetrite t��tlemine
	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '/') || (argv[i][0] == '-') || (argv[i][0] == '\\')) {
			if (sizeof(argv[i]) < 2) continue;
			switch (tolower(argv[i][1])) {
			case 'p':	// pordi m��ramine
				if (i+1 >= argc)
					argumentError();
				checkAgainstBuffer(B_PORT_SIZE, argv[i+1]);
				strcpy(serverPort, argv[++i]);
				break;
			case 's':	// serveri aadressi m��ramine
				if (i+1 >= argc)
					argumentError();
				checkAgainstBuffer(B_HOSTNAME_SIZE, argv[i+1]);
				strcpy(serverHost, argv[++i]);
				break;
			case 'g':	// kirje p�rimine id-j�rgi
				if ((i+1 >= argc) || (procedureToCall != NONE))
					argumentError();
				checkAgainstBuffer(B_INPUT_SIZE, argv[i+1]);
				procedureToCall = GET;
				strcpy(inputBuffer, argv[++i]);
				break;
			case 'l':	// viimase n-i kirje p�rimine
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

	// Kontrollime, kas protseduur on m��ratud
	if (procedureToCall == NONE) {
		fprintf(stderr, "(!!!) Protseduur m��ramata [-a,-l,-g].\n\n");
		printHelp();
		return -1;
	}

	// Koostame "sidumiss�ne"
	status = RpcStringBindingCompose(
		uuid,								// UUID
		(unsigned char*) "ncacn_ip_tcp",	// protokolliks TCP/IP
		(unsigned char*) serverHost,		// Serveri aadress
		(unsigned char*) serverPort,		// Port
		NULL,								// T�iendavaid seadistusi pole
		&pszStringBinding					// Viit muutujale, kuhu salvestada
	);
	if (status)
		rpcError("RpcStringBindingCompose", status);

	// Teeme "sidumiss�nest" "sidumispidemine" :)
	status = RpcBindingFromStringBinding(
		pszStringBinding,					// Viide stringile (sisend)
		&wallOfText_IfHandle				// Viide pidemele (v�ljund)
	);
	if (status)
		rpcError("RpcBindingFromStringBinding", status);

	// Vabastame "sidumiss�ne"
	RpcStringFree(&pszStringBinding);

	// Proovime serverile p�ringut teha
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
				fprintf(stderr, "(!!!) S�numi lisamine eba�nnestus!\n");
			else
				printf("\n\n--> S�num serverile saadetud!\n");
			break;
		}	
	} RpcExcept(1) {
		rpcError("RpcTryExcept", RpcExceptionCode());
	} RpcEndExcept

	// Vabastame pideme
	RpcBindingFree(&wallOfText_IfHandle);
	return 0;
}

/* Kogub d�naamiliselt tekstisisendit.
 * Peaks olema kaitstud bufferi �let�itumise eest.
 * T��tab ainult DOSis/Windowsis (getch()). */
char* gatherDynamicText(void) {
	unsigned int bufferSize = 15;
	unsigned int curLength = 0;
	char *inputBuffer = (char*) malloc(sizeof(char) * bufferSize);
	char nextChar;

	printf("Teksti sisestamise l�petamiseks vajutage [ESC]:\n");
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

// V�tab kasutajalt uue s�numi ning saadab serverisse.
int sendNewMessage(char *author) {
	Message newMessage;
	time_t rawtime;
	char *data;

	if (strlen(author) == 0) {
		fprintf(stderr, "\n\n(!!!) S�numi autor m��ramata!\n");
		return -1;
	}
	data = gatherDynamicText();
	if (strlen(data) == 0) {
		fprintf(stderr, "\n\n(!!!) T�hi s�num ei ole lubatud!\n");
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

// Visatakse, kui ei �nnestu sisendargumente parseda.
__inline void argumentError(void) {
	fprintf(stderr, "(!!!) Viga sisendargumentide t��tlemisel!\n");
	printHelp();
	exit(-1);
}

// P�rib serverilt antud ID-ga s�numit ning v�ljastab.
void getAndPrintMessage(unsigned int messageId) {
	Message message;

	memset(&message, '\0', sizeof(Message));
	if (getMessage(messageId, &message) != 0) {
		fprintf(stderr, "(!!!) Kirje %u p�rimine eba�nnestus. T�en�oliselt seda ei eksisteeri.\n",
			messageId);
	}
	else printMessage(&message);
}

// P�rib serverilt n viimast s�numit ning v�ljastab need.
void getAndPrintMessages(unsigned int n) {
	unsigned int messagesReturned;	// ei pruugi olla nii palju kui tahetakse
	Message **messages = (Message**) midl_user_allocate(sizeof(Message*)*n);

	if (n == 0) {
		fprintf(stderr, "(!!!) Serverit pole m�ttet 0 kirje p�rimisega t��data.\n");
		return;
	}
	memset(messages, '\0', sizeof(Message*)*n);
	
	messagesReturned = getLastMessages(n, messages);
	if (messagesReturned == 0)
		fprintf(stderr, "(!!!) Serveris ei ole �htegi kirjet.\n");
	else {
		if (messagesReturned < n)
			fprintf(stderr, "(!!!) Server saatis v�hem kirjeid kui k�siti. Rohkem ei ole?\n\n");
		printMessages(messages, messagesReturned);
	}

	freeMessagesMemory(messages, n);
}

// Vabastab Messages masiivi m�lu
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

// Vormistab ja v�ljastab s�numi standardv�ljundisse
void printMessage(Message *message) {
	char *hrTime = NULL;
	
	if (message->time != 0) {
		hrTime = asctime(localtime(&message->time));
		// Kustutame l�pust reavahetuse
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

// V�ljastab j�rjest k�ik s�numid massiivis.
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
		"V�imaldab kliendil serverist tellida s�numeid ning enda omasid sinna lisada.\n"
		"Sisendparameetrid:\n\n"
		"-h\t\t- abiinfo (see sama info).\n"
		"-p <port>\t- m��rab pordi, kuhu �ritada �henduda (vaikimisi: 1212).\n"
		"-s <aadress>\t- m��rab serveri aadressi (vaikimisi: localhost).\n\n"
		"M��ratud peab oleva �ks j�rgnevatest protseduuridest:\n"
		"-g <id>\t\t- p�rib <id>le vastavat s�numit.\n"
		"-a <autor>\t- lisab uue s�numi (sisu v�etakse standardsisendist).\n"
		"-l [n]\t\t- tagastab viimased <n> s�numit (vaikimisi: 1).");
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