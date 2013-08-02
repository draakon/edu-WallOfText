/* WallOfText_Server.h :: Teadetetahvli (Wall of Text) server kasutades
 * MS-RPC (DCE RPC) tehnoloogiat.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#pragma once
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <windows.h>
#include <conio.h>
#include "WallOfText.h"
#include "dArray.h"
#include "WallOfText_RP.h"
#include "WallOfText_Database.h"

// Sisendbuffrite suurused
#define B_DATABASE_SIZE 50
#define B_PORT_SIZE 6

extern char databaseFile[B_DATABASE_SIZE];

unsigned long getAndIncCounter(void);