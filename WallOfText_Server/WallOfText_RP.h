/* WallOfText_RP.h :: Wall of Text serveri kaugprotseduurid funktsioonid.
 * Autor: Kristjan Kaitsa
 * Hajussüsteemid (MTAT.08.009) @ 2011 A.D.  */
#pragma once
#include <windows.h>
#include "WallOfText_Server.h"

extern HANDLE mAddMessage;

int getMessage(unsigned int messageId, pMessage outMessage);
unsigned int getLastMessages(unsigned int n, pMessages messages);
void addMessage(pMessage newMessage);
