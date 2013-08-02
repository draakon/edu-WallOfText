/* WallOfText_Client.h :: MS-RPC (DCE RPC) tehnoloogiat kasutava 
 * teadetetahvli (Wall of Text) serveri klient.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#include "WallOfText.h"
#include <locale.h>
#include <stdio.h>
#include <conio.h>

// Sisendbuffrite suurused
#define B_INPUT_SIZE 80
#define B_HOSTNAME_SIZE 50
#define B_PORT_SIZE 6
