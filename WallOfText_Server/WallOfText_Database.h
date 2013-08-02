/* WallOfText_Database.h :: Wall of Text serveri andmebaasi hoidmine, 
 * laadimine, salvestamine.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#include <stdio.h>
#include <string.h>
#include "dArray.h"
#include "WallOfText_Server.h"

extern dArray *database;

int loadDatabase(void);
void saveDatabase(void);
int createBlankDatabase(void);
