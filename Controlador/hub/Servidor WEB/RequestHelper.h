//
//  RequestHelper.h
//  Lumiar-Final
//
//  Created by Denis on 21/06/2016.
//
//

#ifndef RequestHelper_h
#define RequestHelper_h

#include "SizeConstants.h"
#include "FileHelper.h"
#include "../Comum.h"

#include <stdio.h>

void update (char* buffer, char* msg);

char interpretKeyAndValue(char* key, int value);

long getNextKeyAndValue(char* buffer, char* key, int* value, long offset);

STATUS pathFromRequest(char* buffer, char *result);

METODO methodFromRequest(char* buffer);

RESPOSTA interpret (char* buffer, char* msg);

void replaceMessage (char* msg);

void show40x (STATUS status, char* msg);

void findAndReplace(char* haystack, char* needle, int value);

unsigned char isDir (FILE* fd);

STATUS browseAndRead(char* path, char* msg);

#define IF_ERROR_THEN_40X(x) if (status != OK) { show40x(status, msg); return x; }

#endif /* RequestHelper_h */

